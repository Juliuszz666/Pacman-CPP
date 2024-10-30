#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingspage.h"
#include "gameoverpage.h"
#include "gamepage.h"
#include "../shared.h"
#include <QMessageBox>

void criticalQuit(const char * msg)
{
    QMessageBox::critical(nullptr, "Critical error occured", msg);
    QCoreApplication::quit();
    exit(1);
}

void MainWindow::connectButtons()
{
    connect(settings_page, &SettingsPage::backButtonClickedSignal, this, &MainWindow::onBackButtonClicked);
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quitButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClicked);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game_page = new GamePage(this, ui->stackedWidget);
    settings_page = new SettingsPage(this, ui->stackedWidget);
    game_over_page = new GameOverPage(this, ui->stackedWidget);
    ui->stackedWidget->addWidget(game_page);
    ui->stackedWidget->addWidget(settings_page);
    ui->stackedWidget->addWidget(game_over_page);
    ui->stackedWidget->setCurrentWidget(ui->welcome_page);
    Shared::pageIndexStack.push(WELCOME_PAGE);
    connectButtons();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onBackButtonClicked()
{
    Shared::pageIndexStack.clear();
    Shared::pageIndexStack.push(WELCOME_PAGE);
    ui->stackedWidget->setCurrentIndex(WELCOME_PAGE);
    game_page->resetGame();
    Shared::score = 0;
}

void MainWindow::onBackToGame()
{
    //game_page->backToGame();
}

void MainWindow::startButtonClicked()
{
    Shared::pageIndexStack.push(GAME_PAGE);
    ui->stackedWidget->setCurrentIndex(GAME_PAGE);
    game_page->run();
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
