#include "maploader.h"
#include "../shared.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "../MapElements/tile.h"

extern void criticalQuit(const char * msg);

constexpr int FOOD_SCORE = 1;
constexpr int POWER_UP_SCORE = 25;

void MapLoader::loadLevel(int (&mapGrid)[MAP_HEIGHT][MAP_WIDTH], int lvl_number)
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
    QJsonArray gridArray = jsonObj[QString("lvl_%1").arg(lvl_number)].toArray();
    initializeGrid(mapGrid, gridArray);
}

void MapLoader::initializeGrid(int (&mapGrid)[MAP_HEIGHT][MAP_WIDTH], const QJsonArray &jsonArr)
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

int MapLoader::getNoOfLevels()
{
    QFile jsonFile(LEVELS_FILE);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         criticalQuit("Couldn't load level. Program will be terminated");
    }
    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(doc.isNull() || !doc.isObject())
    {
        criticalQuit("Corrupted json file. Program will be terminated");
    }
    return doc.object().keys().size();
}

std::pair<Tile*, Collectable*> MapLoader::processGridValue(int val, std::pair<int, int> pos)
{
    Collectable * item = nullptr;
    Tile * tile = nullptr;
    auto [i, j] = pos;
    switch (val) {
    case MAP_BLANK:
        tile = new Tile(FLOOR, CELL_SIZE, {i, j});
        break;
    case MAP_WALL:
        tile = new Tile(WALL, CELL_SIZE, {i, j});
        break;
    case MAP_FOOD:
        tile = new Tile(FLOOR, CELL_SIZE, {i, j});
        item = new Collectable(FOOD, FOOD_SCORE, CELL_SIZE, {i ,j});
        break;
    case MAP_POWER_UP:
        tile = new Tile(FLOOR, CELL_SIZE, {i, j});
        item = new Collectable(POWER_UP, POWER_UP_SCORE, CELL_SIZE, {i, j});
        break;
    case __GHOST_GATE:
        tile = new Tile(GHOST_GATE, CELL_SIZE, {i, j});
        break;
    default:
        criticalQuit("Level file corrupted");
        break;
    }
    return std::make_pair(tile, item);
}
