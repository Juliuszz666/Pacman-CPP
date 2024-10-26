#include "gameoverpage.h"
#include "./ui_gameoverpage.h"
#include "../shared.h"

GameOverPage::GameOverPage(QWidget *parent, QStackedWidget* _layout_ref)
    : QWidget(parent),
    ui(new Ui::GameOverPage),
    layout_ref(_layout_ref)
{
    ui->setupUi(this);
    connectButtons();
    ui->scoreLabel->setText("You score is: " + QString::number(Shared::score));
    Shared::score = 0;
}

GameOverPage::~GameOverPage()
{
    delete ui;
}

void GameOverPage::connectButtons()
{
    connect(ui->quitButton, &QPushButton::clicked, this, &GameOverPage::quit);
    connect(ui->menuButton, &QPushButton::clicked, this, &GameOverPage::backToMenu);
}

void GameOverPage::backToMenu()
{
    Shared::pageIndexStack.clear();
    Shared::pageIndexStack.push(WELCOME_PAGE);
    layout_ref->setCurrentIndex(WELCOME_PAGE);
}

void GameOverPage::quit()
{
    QCoreApplication::quit();
}


