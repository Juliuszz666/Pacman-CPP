#ifndef PINKY_H
#define PINKY_H
#include "ghost.h"

using path_t = std::vector<std::pair<int,int>>;

class Pinky : public Ghost
{
private:
    void BFSChase();
    path_t BFS(int p_x, int p_y);
public slots:
    void updatePacmanPos();
public:
    Pinky(const int size, const std::pair<int, int> ini_pos);
    ~Pinky() override = default;
    void move() override;
    bool canMove(DirVectors speed_vec) override;
    void returnToSpawn() override;
};

#endif // PINKY_H
