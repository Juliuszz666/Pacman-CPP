#include "gamepage.h"
#include "./ui_gamepage.h"
#include "collectable.h"
#include "shared.h"
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>


#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3
#define LEVELS_FILE ":/levels.json"

void criticalQuit(const char * msg);

GamePage::GamePage(QWidget *parent, QStackedWidget* ref) :
    QWidget(parent),
    ui(new Ui::GamePage),
    current_level(1),
    layout_ref(ref)
{
    ui->setupUi(this);

    loadLevel(current_level);
    drawMapGrid();
}

GamePage::~GamePage()
{
    delete ui;
}

QGraphicsRectItem* setUpRect(int value, int cellSize, std::pair<int, int> pos)
{
    QColor color;

    switch (value) {
    case MAP_WALL:
        color = Qt::black;
        break;
    case MAP_BLANK: case MAP_FOOD: case MAP_POWER_UP:
        color = Qt::white;
        break;
    default:
        criticalQuit("Level file corrupted");
        break;
    }

    auto [y, x] = pos;
    QGraphicsRectItem *rect = new QGraphicsRectItem(x * cellSize, y * cellSize, cellSize, cellSize);

    rect->setBrush(QBrush(color));
    rect->setPen(QPen(Qt::NoPen));

    return rect;
}


void GamePage::drawMapGrid()
{
    scene = new QGraphicsScene(this);

    const int cellSize = 20;

    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            int value = mapGrid[i][j];

            QGraphicsRectItem* rect = setUpRect(value, cellSize, {i, j});
            QGraphicsPixmapItem* map_item = nullptr;
            scene->addItem(rect);
            switch (value) {
            case MAP_FOOD:
                map_item = new Collectable(FOOD, 1, cellSize, {i, j});
                break;
            case MAP_POWER_UP:
                map_item = new Collectable(POWER_UP, 25, cellSize, {i, j});
                break;
            }
            if(map_item)
            {
                scene->addItem(map_item);
            }
        }
    }

    ui->graphicsView->setScene(scene);
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
}
