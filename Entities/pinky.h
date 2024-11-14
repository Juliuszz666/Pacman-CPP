#ifndef PINKY_H
#define PINKY_H
#include "ghost.h"

using path_t = std::vector<std::pair<int,int>>;

class Pinky : public Ghost
{
private:
    void BFSChase();
    path_t BFS(int p_x, int p_y);
    int maze [MAP_HEIGHT][MAP_WIDTH];
public slots:
    void updatePacmanPos();
public:
    Pinky(const int size, const std::pair<int, int> ini_pos, const std::pair<int,int> gate_pos);
    ~Pinky() = default;
    void move() override;
    void load_maze(const int map[MAP_HEIGHT][MAP_WIDTH]);
};

#endif // PINKY_H
