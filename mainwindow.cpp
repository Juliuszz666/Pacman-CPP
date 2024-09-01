#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define GAME_PAGE 0
#define SETTINGS_PAGE 1
#define WELCOME_PAGE 2
#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3
#define LEVELS_FILE ":/levels.json"

void criticalQuit(const char * msg)
{
    QMessageBox::critical(nullptr, "Critical error occured", msg);
    QCoreApplication::quit();
    exit(1);
}

QGraphicsRectItem* setUpRect(int value, int cellSize, std::pair<int, int> pos)
{
    QColor color;

    switch (value) {
    case MAP_WALL:
        color = Qt::black;
        break;
    case MAP_BLANK: case MAP_FOOD: case MAP_POWER_UP:
        color = Qt::white;
        break;
    default:
        criticalQuit("Level file corrupted");
        break;
    }

    auto [i, j] = pos;
    QGraphicsRectItem *rect = new QGraphicsRectItem(j * cellSize, i * cellSize, cellSize, cellSize);

    rect->setBrush(QBrush(color));
    rect->setPen(QPen(Qt::NoPen));

    return rect;
}

QGraphicsPixmapItem * setUpCollectable(const int cellSize, std::pair<int, int> pos, const QString& filename)
{
    QImage img;
    if(!img.load(filename))
    {
        criticalQuit("Error loading an image");
    }
    QPixmap loaded_img = QPixmap::fromImage(img);
    auto [y, x] = pos;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(loaded_img.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
    item->setPos(x * cellSize, y * cellSize);
    return item;

}

void MainWindow::drawMapGrid()
{
    scene = new QGraphicsScene(this);

    const int cellSize = 20;

    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            int value = mapGrid[i][j];

            QGraphicsRectItem* rect = setUpRect(value, cellSize, {i, j});
            QGraphicsPixmapItem* map_item = nullptr;
            scene->addItem(rect);
            switch (value) {
            case MAP_FOOD:
                map_item = setUpCollectable(cellSize, {i, j}, ":/img/food.png");
                break;
            case MAP_POWER_UP:
                map_item = setUpCollectable(cellSize, {i, j}, ":/img/power_up.png");
                break;
            }
            if(map_item)
            {
                scene->addItem(map_item);
            }
        }
    }

    ui->graphicsView->setScene(scene);
}

void MainWindow::initializeGrid(const QJsonArray &jsonArr)
{
    if (jsonArr.size() != MAP_HEIGHT || jsonArr[0].toArray().size() != MAP_WIDTH)
    {
        criticalQuit("File with level corrupted");
    }
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        QJsonArray row = jsonArr[i].toArray();
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            mapGrid[i][j] = row[j].toInt();
        }
    }
}


void MainWindow::loadLevel(int level_number)
{
    QFile jsonFile(LEVELS_FILE);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        criticalQuit("Couldn't load level. Program will be terminated");
    }
    QByteArray fileData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument document = QJsonDocument::fromJson(fileData);
    if (!document.isObject()) {
        criticalQuit("Couldn't load level. Program will be terminated");
    }

    QJsonObject jsonObj = document.object();
    QJsonArray gridArray = jsonObj[QString("lvl_%1").arg(level_number)].toArray();
    initializeGrid(gridArray);
}

void MainWindow::checkDuplicates()
{
    QSet<int> checkSet;
    for(auto element = keyBindings.constBegin(); element != keyBindings.constEnd(); ++element)
    {
        if(checkSet.contains(element.value()))
        {
            keyBindings = defaultBindings;
            setUpButtonActions();
            QMessageBox::critical(nullptr, "Error", "Error with key binding, restored to default");
            return;
        }
        else
        {
            checkSet.insert(element.value());
        }
    }
}

void MainWindow::pushPage(int index)
{
    pageIndexStack.push(ui->stackedWidget->currentIndex());
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::popPage()
{
    if (!pageIndexStack.isEmpty())
    {
        int previousPage = pageIndexStack.pop();
        ui->stackedWidget->setCurrentIndex(previousPage);
    }
}

void MainWindow::setUpMusic()
{
    music_player->setAudioOutput(audio_output);
    music_player->setSource(QUrl("qrc:/mp3/testmusic.mp3"));
    music_player->play();
}

void MainWindow::changeVolume()
{
    qreal vol = QAudio::convertVolume(ui->volumeSlider->value() / qreal(100.0), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    audio_output->setVolume(vol);
}

void MainWindow::connectButtons()
{
    /* MENU BUTTONS */
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quitButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::backButtonClicked);
    /* BINDING BUTTONS */
    connect(ui->mvDownBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvUpBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvLeftBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvRightBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->pauseBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->settingsBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    /* VOLUME SLIDER */
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::changeVolume);
}

void MainWindow::setUpButtonActions()
{
    buttonActions.insert(ui->mvDownBinder, MVDOWN);
    ui->mvDownBinder->setText(QKeySequence(keyBindings[MVDOWN]).toString());
    buttonActions.insert(ui->mvUpBinder, MVUP);
    ui->mvUpBinder->setText(QKeySequence(keyBindings[MVUP]).toString());
    buttonActions.insert(ui->mvLeftBinder, MVLEFT);
    ui->mvLeftBinder->setText(QKeySequence(keyBindings[MVLEFT]).toString());
    buttonActions.insert(ui->mvRightBinder, MVRIGHT);
    ui->mvRightBinder->setText(QKeySequence(keyBindings[MVRIGHT]).toString());
    buttonActions.insert(ui->pauseBinder, PAUSE);
    ui->pauseBinder->setText(QKeySequence(keyBindings[PAUSE]).toString());
    buttonActions.insert(ui->settingsBinder, SETTINGS);
    ui->settingsBinder->setText(QKeySequence(keyBindings[SETTINGS]).toString());
    /*VOLUME SLIDER*/
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentButton(nullptr),
    currentLevel(1),
    ghosts(nullptr), //temp
    music_player(new QMediaPlayer()),
    audio_output(new QAudioOutput()),
    pacman(nullptr), //temp
    timer(nullptr), //temp
    scene(nullptr)
{
    ui->setupUi(this);

    keyBindings = defaultBindings;
    setUpButtonActions();
    connectButtons();
    setUpMusic();

    loadLevel(1);
    drawMapGrid();

    pageIndexStack.push(WELCOME_PAGE);
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);
}

MainWindow::~MainWindow() {
    delete ui;
    delete scene;
    delete music_player;
    delete audio_output;
}

void MainWindow::startButtonClicked()
{
    pushPage(GAME_PAGE);
}

void MainWindow::quitButtonClicked()
{
    QCoreApplication::quit();
}

void MainWindow::backButtonClicked()
{
    while(pageIndexStack.top() != WELCOME_PAGE)
    {
        popPage();
    }
    popPage();
}

void MainWindow::settingsButtonClicked()
{
    pushPage(SETTINGS_PAGE);
}

void MainWindow::bindKey()
{
    if (currentButton)
    {
        keyActions action = buttonActions[currentButton];
        currentButton->setText(QKeySequence(keyBindings[action]).toString());
    }
    currentButton = qobject_cast<QPushButton*>(sender());
    if(currentButton)
    {
        currentButton->setText("Press key...");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch (ui->stackedWidget->currentIndex())
    {
    case SETTINGS_PAGE:
        settingsPageKeysHandling(key);
        break;
    case GAME_PAGE:
        gamePageKeysHandling(key);
        break;
    }
}

void MainWindow::settingsPageKeysHandling(int key)
{
    if(currentButton)
    {
        keyActions action = buttonActions[currentButton];
        for(auto it = keyBindings.constBegin(); it != keyBindings.constEnd(); ++it)
        {
            if(it.value() == key)
            {
                keyBindings[action] = defaultBindings[action];
                currentButton->setText(QKeySequence(defaultBindings[action]).toString());
                checkDuplicates();
                return;
            }
        }
        keyBindings[action] = key;
        currentButton->setText(QKeySequence(key).toString());
        currentButton = nullptr;
    }
    else
    {
        if(key == keyBindings[SETTINGS])
        {
            popPage();
            if(ui->stackedWidget->currentIndex() != GAME_PAGE) pushPage(SETTINGS_PAGE);
        }
    }
}

void MainWindow::gamePageKeysHandling(int key)
{
    if(key == keyBindings[SETTINGS])
    {
        pushPage(SETTINGS_PAGE);
    }
}
