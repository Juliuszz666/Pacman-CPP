#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define GAME_PAGE 0
#define SETTINGS_PAGE 1
#define WELCOME_PAGE 2

void MainWindow::connectButtons()
{
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quitButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClicked);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::settingsButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}
