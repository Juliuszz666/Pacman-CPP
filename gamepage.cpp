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



#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3
#define LEVELS_FILE ":/levels.json"

void criticalQuit(const char * msg);
const int cellSize = 20;

GamePage::GamePage(QWidget *parent, QStackedWidget* ref) :
    QWidget(parent),
    ui(new Ui::GamePage),
    current_level(1),
    layout_ref(ref),
    pacman(new Pacman(cellSize, {1, 1})),
    scene(new QGraphicsScene(this)),
    player_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    loadLevel(current_level);
    drawMapGrid();
    scene->addItem(pacman);

    player_timer->start(100);

    connect(player_timer, &QTimer::timeout, pacman, &Pacman::move);
}

GamePage::~GamePage()
{
    delete ui;
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
                map_tiles.push_back(tile);
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
    if(key == Shared::keyBindings[SETTINGS])
    {
        Shared::pageIndexStack.push(SETTINGS_PAGE);
        layout_ref->setCurrentIndex(SETTINGS_PAGE);
    }
    else if (key == Shared::keyBindings[MVUP])
    {
        pacman->setDir(UP);
    }
    else if (key == Shared::keyBindings[MVLEFT])
    {
        pacman->setDir(LEFT);
    }
    else if (key == Shared::keyBindings[MVDOWN])
    {
        pacman->setDir(DOWN);
    }
    else if (key == Shared::keyBindings[MVRIGHT])
    {
        pacman->setDir(RIGHT);
    }

}
