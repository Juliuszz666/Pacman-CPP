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

void MainWindow::drawMapGrid()
{
    scene = new QGraphicsScene(this);

    const int cellSize = 20;

    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            int value = mapGrid[i][j];

            QColor color;
            switch (value) {
            case MAP_BLANK:
                color = Qt::white;
                break;
            case MAP_WALL:
                color = Qt::black;
                break;
            case MAP_POWER_UP:
                color = Qt::yellow;
                break;
            case MAP_FOOD:
                color = Qt::green;
                break;
            default:
                criticalQuit("Level file corrupted");
                break;
            }

            QGraphicsRectItem *rect = new QGraphicsRectItem(j * cellSize, i * cellSize, cellSize, cellSize);

            rect->setBrush(QBrush(color));
            rect->setPen(QPen(Qt::NoPen));

            scene->addItem(rect);
        }
    }

    // Set the scene to the QGraphicsView
    ui->graphicsView->setScene(scene);

    // Optional: Adjust the view to show the entire grid
    ui->graphicsView->setSceneRect(0, 0, MAP_WIDTH * cellSize, MAP_HEIGHT * cellSize);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
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
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentButton(nullptr),
    currentLevel(1)
{
    ui->setupUi(this);

    keyBindings = defaultBindings;
    setUpButtonActions();
    connectButtons();

    loadLevel(1);
    drawMapGrid();

    pageIndexStack.push(WELCOME_PAGE);
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);
}

MainWindow::~MainWindow() {
    delete ui;
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
