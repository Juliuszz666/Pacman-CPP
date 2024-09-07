#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingspage.h"
#include "gamepage.h"
#include "shared.h"
#include <QMessageBox>

void criticalQuit(const char * msg)
{
    QMessageBox::critical(nullptr, "Critical error occured", msg);
    QCoreApplication::quit();
    exit(1);
}

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
    connectButtons();

    GamePage* game_page = new GamePage(this, ui->stackedWidget);
    SettingsPage* settings_page = new SettingsPage(this, ui->stackedWidget);
    ui->stackedWidget->addWidget(game_page);
    ui->stackedWidget->addWidget(settings_page);
    ui->stackedWidget->setCurrentWidget(ui->welcome_page);
    Shared::pageIndexStack.push(WELCOME_PAGE);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startButtonClicked()
{
    Shared::pageIndexStack.push(GAME_PAGE);
    ui->stackedWidget->setCurrentIndex(GAME_PAGE);
}

void MainWindow::quitButtonClicked()
{
    QCoreApplication::quit();
}

void MainWindow::settingsButtonClicked()
{
    Shared::pageIndexStack.push(SETTINGS_PAGE);
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}
