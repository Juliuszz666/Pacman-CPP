#include "gamepage.h"
#include "./ui_gamepage.h"
#include "../shared.h"
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTimer>
#include "../MapElements/collectable.h"
#include "../MapElements/tile.h"
#include "../Entities/blinky.h"
#include "../Entities/clyde.h"
#include "../Entities/inky.h"
#include "../Entities/pinky.h"

void criticalQuit(const char * msg);
const int cellSize = 20;

GamePage::GamePage(QWidget *parent, QStackedWidget* ref) :
    QWidget(parent),
    ui(new Ui::GamePage),
    current_level(1),
    layout_ref(ref),
    pacman(new Pacman(cellSize, {1, 1})),
    scene(new QGraphicsScene(this)),
    player_timer(new QTimer(this)),
    power_up_timer(new QTimer(this)),
    ghosts
    {
        new Blinky(cellSize, {7,15}, {5, 15}),
        new Clyde(cellSize, {7, 14}, {5, 15}),
        new Inky(cellSize, {7, 16}, {5, 15}),
        new Pinky(cellSize, {5, 15}, {5, 15})
    }
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    loadLevel(current_level);
    drawMapGrid();
    Shared::score = 0;
    scene->addItem(pacman);
    for (int i = 0; i < NO_OF_GHOSTS; ++i)
    {
        scene->addItem(ghosts[i]);
    }
    connectTimers();
}

void GamePage::connectTimers()
{
    Blinky* blinky =    static_cast<Blinky*>(ghosts[0]);
    Clyde* clyde =      static_cast<Clyde*>(ghosts[1]);
    Inky* inky =        static_cast<Inky*>(ghosts[2]);
    Pinky* pinky =      static_cast<Pinky*>(ghosts[3]);
    pinky->getPacmanPos(pacman->pos().toPoint());
    pinky->load_maze(mapGrid);
    connect(player_timer,   &QTimer::timeout, pacman,           &Pacman::move);
    connect(player_timer,   &QTimer::timeout, this,             &GamePage::handlePacmanCollision);
    connect(player_timer,   &QTimer::timeout, this,             &GamePage::updateScore);
    connect(player_timer,   &QTimer::timeout, pacman,           &Pacman::canChangeDir);
    connect(power_up_timer, &QTimer::timeout, power_up_timer,   &QTimer::stop);
    connect(power_up_timer, &QTimer::timeout, this,             &GamePage::endPowerUpMode);
    connect(player_timer,   &QTimer::timeout, blinky,           &Blinky::move);
    connect(player_timer,   &QTimer::timeout, clyde,            &Clyde::move);
    connect(player_timer,   &QTimer::timeout, inky,             &Inky::move);
    connect(player_timer,   &QTimer::timeout, pinky,            &Pinky::move);
    connect(player_timer,   &QTimer::timeout, this,             [&]() {
        for (int i = 0; i < NO_OF_GHOSTS; ++i)
        {
            ghosts[i]->getPacmanPos(pacman->pos().toPoint());
        }
    });
}

void GamePage::run()
{
    player_timer->start(TIMER_COLAPSE_TIME);
}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::updateScore()
{
    ui->label->setText("SCORE: " + QString::number(Shared::score));
}

std::pair<Tile*, Collectable*> processGridValue(int val, std::pair<int, int> pos)
{
    Collectable * item = nullptr;
    Tile * tile = nullptr;
    auto [i, j] = pos;
    switch (val) {
    case MAP_BLANK:
        tile = new Tile(FLOOR, cellSize, {i, j});
        break;
    case MAP_WALL:
        tile = new Tile(WALL, cellSize, {i, j});
        break;
    case MAP_FOOD:
        tile = new Tile(FLOOR, cellSize, {i, j});
        item = new Collectable(FOOD, 1, cellSize, {i ,j});
        break;
    case MAP_POWER_UP:
        tile = new Tile(FLOOR, cellSize, {i, j});
        item = new Collectable(POWER_UP, 25, cellSize, {i, j});
        break;
    case __GHOST_GATE:
        tile = new Tile(GHOST_GATE, cellSize, {i, j});
        break;
    default:
        criticalQuit("Level file corrupted");
        break;
    }
    return std::make_pair(tile, item);
}

void GamePage::drawMapGrid()
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            auto [tile, item] = processGridValue(mapGrid[i][j], {i, j});
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

void GamePage::initializeGrid(const QJsonArray &jsonArr)
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


void GamePage::loadLevel(int level_number)
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

void GamePage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    static bool paused = false;
    if(key == Shared::keyBindings[SETTINGS])
    {
        Shared::pageIndexStack.push(SETTINGS_PAGE);
        layout_ref->setCurrentIndex(SETTINGS_PAGE);
    }
    moveDirections new_dir = NONE;
    if(key == Shared::keyBindings[MVUP])            new_dir = UP;
    else if(key == Shared::keyBindings[MVLEFT])     new_dir = LEFT;
    else if(key == Shared::keyBindings[MVDOWN])     new_dir = DOWN;
    else if(key == Shared::keyBindings[MVRIGHT])    new_dir = RIGHT;
    else if (key == Shared::keyBindings[PAUSE] && paused)
    {
        player_timer->start(TIMER_COLAPSE_TIME);
        paused = false;
    }
    else if (key == Shared::keyBindings[PAUSE] && !paused)
    {
        player_timer->stop();
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

void GamePage::handlePacmanCollision()
{
    QList<QGraphicsItem*> collisions = pacman->collidingItems();
    collectCollectables(collisions);
    ghostCollisions(collisions);
}

void GamePage::newLifeRestart()
{
    pacman->reset();
    for (auto ghost : ghosts)
    {
        ghost->returnToSpawn();
    }
}

void GamePage::ghostCollisions(const QList<QGraphicsItem*> &collisions)
{
    for (const auto &item : collisions)
    {
        Ghost* ghost = dynamic_cast<Ghost*>(item);
        qDebug() << (void*)ghost;
        if(ghost)
        {
            switch (ghost->getState()) {
            case EDIBLE:
                ghost->returnToSpawn();
                Shared::score += 200; // placeholder
                break;
            case INEDIBLE:
                if(pacman->loseLife())
                {
                    newLifeRestart();
                }
                else
                {
                    gameOver();
                    return;
                }
                break;
            }
        }
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
    loadLevel(current_level);
    drawMapGrid();
    for (auto ghost : ghosts)
    {
        ghost->returnToSpawn();
        ghost->setZValue(1.0);
    }
    pacman->reset();
}

void GamePage::collectCollectables(const QList<QGraphicsItem*> &collisions)
{
    for (const auto &item : collisions)
    {
        Collectable* collectable = dynamic_cast<Collectable*>(item);
        if (collectable)
        {
            if(collectable->getType() == POWER_UP)
            {
                powerUpMode();
            }
            auto it = std::find(collectables.begin(), collectables.end(), collectable);
            Shared::score += collectable->getScore();
            scene->removeItem(item);
            collectables.erase(it);
        }
    }
    if(collectables.empty())
    {
        ui->label->setText("KONIEC");
    }
}
