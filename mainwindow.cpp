#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

#define GAME_PAGE 0
#define SETTINGS_PAGE 1
#define WELCOME_PAGE 2

void MainWindow::connectButtons()
{
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quitButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::backButtonClicked);

    connect(ui->mvDownBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvUpBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvLeftBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->mvRightBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->pauseBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
    connect(ui->settingsBinder, &QPushButton::clicked, this, &MainWindow::bindKey);
}

void MainWindow::setUpButtonActions()
{
    buttonActions.insert(ui->mvDownBinder, "MVDOWN");
    buttonActions.insert(ui->mvUpBinder, "MVUP");
    buttonActions.insert(ui->mvLeftBinder, "MVLEFT");
    buttonActions.insert(ui->mvRightBinder, "MVRIGHT");
    buttonActions.insert(ui->pauseBinder, "PAUSE");
    buttonActions.insert(ui->settingsBinder, "SETTINGS");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpButtonActions();

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 100, 100);
    rect->setBrush(QBrush(Qt::blue));
    scene->addItem(rect);
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);

    connectButtons();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(GAME_PAGE);
}

void MainWindow::quitButtonClicked()
{
    QCoreApplication::quit();
}

void MainWindow::backButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);
}

void MainWindow::settingsButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}

void MainWindow::bindKey()
{
    currentButton = qobject_cast<QPushButton*>(sender());
    if(currentButton)
    {
        currentButton->setText("Press key...");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(currentButton)
    {
        int key = event->key();
        QString action = buttonActions[currentButton];
        for(auto it = keyBindings.constBegin(); it != keyBindings.constEnd(); ++it)
        {
            if(it.value() == key)
            {
                keyBindings[action] = defaultBindings[action];
                currentButton->setText(QKeySequence(defaultBindings[action]).toString());
                return;
            }
        }
        QMessageBox::critical(nullptr, "Chuj", QString::number(key));
        keyBindings[action] = key;
        currentButton->setText(QKeySequence(key).toString());
        currentButton = nullptr;
    }
}
