#include "gamepage.h"
#include "./ui_gamepage.h"
#include "shared.h"
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTimer>
#include "collectable.h"
#include "tile.h"
#include "blinky.h"
#include "clyde.h"
#include "inky.h"
#include "pinky.h"

#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3
#define LEVELS_FILE ":/levels.json"
#define TIMER_COLAPSE_TIME (1000/30.0)      // 30 FPS
#define POWER_UP_TIME 10'000                //ms

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
    score(0),
    ghosts
    {
        new Blinky(cellSize, {8,14}),
        new Clyde(cellSize, {8, 15}),
        new Inky(cellSize, {9, 14}),
        new Pinky(cellSize, {18, 3})
    }
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    loadLevel(current_level);
    drawMapGrid();
    scene->addItem(pacman);
    for (int i = 0; i < NO_OF_GHOSTS; ++i)
    {
        scene->addItem(ghosts[i]);
    }

    player_timer->start(TIMER_COLAPSE_TIME);

    connect(player_timer, &QTimer::timeout, pacman, &Pacman::move);
    connect(player_timer, &QTimer::timeout, this, &GamePage::handlePacmanCollision);
    connect(player_timer, &QTimer::timeout, this, &GamePage::updateScore);
    connect(player_timer, &QTimer::timeout, pacman, &Pacman::canChangeDir);
    connect(power_up_timer, &QTimer::timeout, power_up_timer, &QTimer::stop);
    connect(power_up_timer, &QTimer::timeout, this, &GamePage::endPowerUpMode);
}

GamePage::~GamePage()
{
    delete ui;
    for(auto ghost : ghosts)
    {
        delete ghost;
    }
}

void GamePage::updateScore()
{
    ui->label->setText("SCORE: " + QString::number(score));
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
    else if (key == Shared::keyBindings[MVUP])
    {
        if(!pacman->setDir(UP))
        {
            pacman->setNextDir(UP);
        }
    }
    else if (key == Shared::keyBindings[MVLEFT])
    {
        if(!pacman->setDir(LEFT))
        {
            pacman->setNextDir(LEFT);
        }
    }
    else if (key == Shared::keyBindings[MVDOWN])
    {
        if(!pacman->setDir(DOWN))
        {
            pacman->setNextDir(DOWN);
        }
    }
    else if (key == Shared::keyBindings[MVRIGHT])
    {
        if(!pacman->setDir(RIGHT))
        {
            pacman->setNextDir(RIGHT);
        }
    }
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

// everything in switch cases to be done
void GamePage::ghostCollisions(const QList<QGraphicsItem*> &collisions)
{
    qDebug() << pacman->getLife();
    qDebug() << ghosts[3]->getState();
    for (const auto &item : collisions)
    {
        Ghost* ghost = dynamic_cast<Ghost*>(item);
        if(ghost)
        {
            switch (ghost->getState()) {
            case EDIBLE:
                ghost->returnToSpawn();
                score += 200; // placeholder
                break;
            case INEDIBLE:
                if(pacman->loseLife())
                {
                    resetGame();
                }
                else
                {
                    gameOver();
                }
                break;
            }
        }
    }
}

void GamePage::gameOver()
{
    qDebug() << "Zrób to";
}

void GamePage::resetGame()
{
    pacman->setPos(cellSize, cellSize);
    for (auto ghost : ghosts) {
        ghost->returnToSpawn();
    }
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
            score += collectable->getScore();
            scene->removeItem(item);
            collectables.erase(it);
        }
    }
    if(collectables.empty())
    {
        ui->label->setText("KONIEC");
    }
}
