#ifndef MAPLOADER_H
#define MAPLOADER_H
#include "../shared.h"
#include "../MapElements/tile.h"
#include "../MapElements/collectable.h"

#include <QJsonArray>

class MapLoader
{
public:
    static std::pair<Tile*, Collectable*> processGridValue(int val, std::pair<int, int> pos);
    static void loadLevel(int (&mapGrid)[MAP_HEIGHT][MAP_WIDTH], int lvl_number);
    static void initializeGrid(int (&grid)[MAP_HEIGHT][MAP_WIDTH], const QJsonArray &jsonArr);
    static int getNoOfLevels();
    MapLoader() = default;
};

#endif // MAPLOADER_H
