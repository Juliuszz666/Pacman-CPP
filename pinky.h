#ifndef PINKY_H
#define PINKY_H
#include "ghost.h"

#define MAP_WIDTH 30
#define MAP_HEIGHT 20

class Pinky : public Ghost
{
private:
    int maze [MAP_HEIGHT][MAP_WIDTH];
    void BFSChase();
public slots:
    void updatePacmanPos();
public:
    Pinky(const int size, const std::pair<int, int> ini_pos);
    ~Pinky() override = default;
    void move() override;
    bool canMove(DirVectors speed_vec) override;
    void load_maze(const int maze[MAP_HEIGHT][MAP_WIDTH]);
    void returnToSpawn() override;
};

#endif // PINKY_H
