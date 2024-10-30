#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <QJsonArray>

class MapLoader
{
public:
    static void drawMapGrid();
    static void loadLevel(int lvl_number);
    static void initializeGrid(const QJsonArray &jsonArr);
    MapLoader() = default;
};

#endif // MAPLOADER_H
