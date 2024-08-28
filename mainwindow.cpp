#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSet>

#define GAME_PAGE 0
#define SETTINGS_PAGE 1
#define WELCOME_PAGE 2

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
    currentButton(nullptr)
{
    ui->setupUi(this);

    keyBindings = defaultBindings;
    setUpButtonActions();

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 100, 100);
    rect->setBrush(QBrush(Qt::blue));
    scene->addItem(rect);
    pageIndexStack.push(WELCOME_PAGE);
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);
    connectButtons();

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
