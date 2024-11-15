#include "gamepage.h"
#include "./ui_gamepage.h"
#include "../shared.h"
#include "maploader.h"
#include "../MapElements/collectable.h"
#include "../MapElements/tile.h"
#include "../Entities/blinky.h"
#include "../Entities/clyde.h"
#include "../Entities/inky.h"
#include "../Entities/pinky.h"
#include "collisionhandler.h"

#define CURRENT_LEVEL_STR "Level: " + QString::number(current_level)
#define CURRENT_SCORE_STR "SCORE: " + QString::number(Shared::score)
#define HEART_FILE_STR ":/img/heart.png"
#define DEFAULT_PINKY_SPAWN {7, 13}
#define DEFAULT_BLINKY_SPAWN {7, 15}
#define DEFAULT_INKY_SPAWN {7, 16}
#define DEFAULT_CLYDE_SPAWN {7, 14}
#define GHOST_GATE_POS {5, 15}
#define DEFAULT_PACMAN_SPAWN {1, 1}

extern void criticalQuit(const char * msg);

GamePage::GamePage(QWidget *parent, QStackedWidget* ref) :
    QWidget(parent),
    ui(new Ui::GamePage),
    current_level(1),
    layout_ref(ref),
    pacman(new Pacman(CELL_SIZE, DEFAULT_PACMAN_SPAWN)),
    scene(new QGraphicsScene(this)),
    game_timer(new QTimer(this)),
    power_up_timer(new QTimer(this)),
    max_level(MapLoader::getNoOfLevels()),
    collision_handler(new CollisionHandler()),
    ghosts
    {
        new Blinky  (CELL_SIZE, DEFAULT_BLINKY_SPAWN,   GHOST_GATE_POS),
        new Clyde   (CELL_SIZE, DEFAULT_CLYDE_SPAWN,    GHOST_GATE_POS),
        new Inky    (CELL_SIZE, DEFAULT_INKY_SPAWN,     GHOST_GATE_POS),
        new Pinky   (CELL_SIZE, DEFAULT_PINKY_SPAWN,    GHOST_GATE_POS)
    }
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->lvlLabel->setText("Level: " + QString::number(current_level));
    MapLoader::loadLevel(mapGrid, current_level);
    drawMapGrid();
    Shared::score = 0;
    scene->addItem(pacman);
    for (int i = 0; i < NO_OF_GHOSTS; ++i)
    {
        scene->addItem(ghosts[i]);
    }
    connectTimers();
}

GamePage::~GamePage()
{
    delete ui;
    delete collision_handler;
}

void GamePage::connectTimers()
{
    Pinky* pinky =      static_cast<Pinky*>(ghosts[3]);
    pinky->getPacmanPos(pacman->pos().toPoint());
    pinky->load_maze(mapGrid);
    connect(game_timer,     &QTimer::timeout, this,             &GamePage::updateLives);
    connect(game_timer,     &QTimer::timeout, pacman,           &Pacman::move);
    connect(game_timer,     &QTimer::timeout, this,             &GamePage::callCollisionHandler);
    connect(game_timer,     &QTimer::timeout, this,             &GamePage::updateScore);
    connect(game_timer,     &QTimer::timeout, pacman,           &Pacman::canChangeDir);
    connect(power_up_timer, &QTimer::timeout, power_up_timer,   &QTimer::stop);
    connect(power_up_timer, &QTimer::timeout, this,             &GamePage::endPowerUpMode);
    connect(game_timer,     &QTimer::timeout, this,             [&]() {
        for (int i = 0; i < NO_OF_GHOSTS; ++i)
        {
            ghosts[i]->move();
            ghosts[i]->getPacmanPos(pacman->pos().toPoint());
        }
    });
}

void GamePage::updateLives()
{
    constexpr int HEART_POS_CO = 30;
    for (auto heart : heart_icons) {
        scene->removeItem(heart);
        delete heart;
    }
    heart_icons.clear();
    for (int i = 0; i < pacman->getLife(); ++i) {
        QGraphicsPixmapItem *heart = new QGraphicsPixmapItem(QPixmap(HEART_FILE_STR).scaled(CELL_SIZE, CELL_SIZE));
        heart->setPos(i * HEART_POS_CO, -HEART_POS_CO);
        scene->addItem(heart);
        heart_icons.append(heart);
    }
}

void GamePage::callCollisionHandler()
{
    this->collision_handler->handlePacmanCollisions(this);
}

void GamePage::run()
{
    game_timer->start(TIMER_COLAPSE_TIME);
}

void GamePage::updateScore()
{
    ui->label->setText(CURRENT_SCORE_STR);
}

void GamePage::backToGame()
{
    game_timer->start(TIMER_COLAPSE_TIME);
}

void GamePage::drawMapGrid()
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            auto [tile, item] = MapLoader::processGridValue(mapGrid[i][j], {i, j});
            if(tile)
            {
                scene->addItem(tile);
            }
            if(item)
            {
                scene->addItem(item);
                collectables.push_back(item);
            }
        }
    }
}

void GamePage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    static bool paused = false;
    if(key == Shared::keyBindings[SETTINGS])
    {
        game_timer->stop();
        Shared::pageIndexStack.push(SETTINGS_PAGE);
        layout_ref->setCurrentIndex(SETTINGS_PAGE);
    }
    moveDirections new_dir = NONE;
    if(key == Shared::keyBindings[MVUP])            new_dir = UP;
    else if(key == Shared::keyBindings[MVLEFT])     new_dir = LEFT;
    else if(key == Shared::keyBindings[MVDOWN])     new_dir = DOWN;
    else if(key == Shared::keyBindings[MVRIGHT])    new_dir = RIGHT;
    else if(key == Shared::keyBindings[PAUSE] &&    paused)
    {
        game_timer->start(TIMER_COLAPSE_TIME);
        paused = false;
    }
    else if (key == Shared::keyBindings[PAUSE] && !paused)
    {
        game_timer->stop();
        paused = true;
    }
    if(!pacman->setDir(new_dir))
    {
        pacman->setNextDir(new_dir);
    }
}

void GamePage::endPowerUpMode()
{
    for(auto ghost : ghosts)
    {
        ghost->setState(INEDIBLE);
    }
}

void GamePage::powerUpMode()
{
    power_up_timer->start(POWER_UP_TIME);
    for(auto ghost : ghosts)
    {
        ghost->setState(EDIBLE);
    }
}

void GamePage::newLifeRestart()
{
    pacman->reset();
    for (auto ghost : ghosts)
    {
        ghost->returnToSpawn();
    }
}

void GamePage::gameOver()
{
    resetGame();
    Shared::pageIndexStack.push(GAME_OVER_PAGE);
    layout_ref->setCurrentIndex(GAME_OVER_PAGE);
}

void GamePage::resetGame()
{
    current_level = 1;
    foreach(QGraphicsItem *item, scene->items())
    {
        if (dynamic_cast<Collectable*>(item) || dynamic_cast<Tile*>(item))
        {
            scene->removeItem(item);
            delete item;
        }
    }
    MapLoader::loadLevel(mapGrid, current_level);
    drawMapGrid();
    for (auto ghost : ghosts)
    {
        ghost->returnToSpawn();
        ghost->setZValue(1.0);
    }
    pacman->reset();
    pacman->setLives(PACMAN_LIVES_DEFAULT);
}

void GamePage::newLevel()
{
    current_level++;
    MapLoader::loadLevel(mapGrid, current_level);
    drawMapGrid();
    resetGame();
    ui->lvlLabel->setText(CURRENT_LEVEL_STR);
}
