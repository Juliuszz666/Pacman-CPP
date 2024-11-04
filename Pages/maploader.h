#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <QJsonArray>

class MapLoader
{
public:
    static void loadLevel(int (&mapGrid)[20][30], int lvl_number);
    static void initializeGrid(int (&grid)[20][30], const QJsonArray &jsonArr);
    MapLoader() = default;
};

#endif // MAPLOADER_H
