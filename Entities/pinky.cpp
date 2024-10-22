#include "pinky.h"
#include "ghost.h"
#include <QTimer>
#include <queue>

Pinky::Pinky(const int size, const std::pair<int, int> ini_pos, const std::pair<int,int> gate_pos) :
               Ghost(size, ini_pos, ":/img/pinky.png", gate_pos)
{
}

void Pinky::load_maze(const int map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            maze[i][j] = map[i][j] == 1 ? 0 : 1;
        }
    }
    int pac_x = pacman_pos.x() / size;
    int pac_y = pacman_pos.y() / size;
    maze[pac_y][pac_x] = 2;
}

void Pinky::updatePacmanPos()
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            maze[i][j] = maze[i][j] == 2 ? 1 : maze[i][j];
        }
    }
    maze
        [static_cast<int>(std::floor(pacman_pos.y() / size))]
        [static_cast<int>(std::floor(pacman_pos.x() / size))] = 2;
}

path_t Pinky::BFS(int pinky_x, int pinky_y)
{
    bool visited[MAP_HEIGHT][MAP_WIDTH] = {false};
    path_t                  path;
    std::queue<path_t>      BFS_queue;
    std::function<bool(int, int)> is_wall = [this, &visited](int y, int x)
    {
        if(x < 0 || y < 0 || x > MAP_WIDTH - 1 || y > MAP_HEIGHT - 1) return true;
        return !(maze[y][x] ^ visited[y][x]);
    };
    std::function<void(const path_t&, int, int, int, int)> visit_tile =
        [this, &BFS_queue, &visited]
        (const path_t& curr_path, int y, int x, int dy, int dx)
    {
        visited[y][x] = true;
        path_t new_path(curr_path);
        new_path.emplace_back(std::make_pair(y + dy, x + dx));
        BFS_queue.push(new_path);
    };
    BFS_queue.push({std::make_pair(pinky_y, pinky_x)});
    path_t best_path;
    while(!BFS_queue.empty())
    {
        path_t curr_path = BFS_queue.front();
        BFS_queue.pop();
        auto [cur_y, cur_x] = curr_path.back();
        if(maze[cur_y][cur_x] == 2)
        {
            best_path = curr_path;
            break;
        }
        if(!is_wall(cur_y - 1, cur_x))
        {
            visit_tile(curr_path, cur_y, cur_x, -1, 0);
        }
        if(!is_wall(cur_y + 1, cur_x))
        {
            visit_tile(curr_path, cur_y, cur_x, 1, 0);
        }
        if(!is_wall(cur_y, cur_x - 1))
        {
            visit_tile(curr_path, cur_y, cur_x, 0, -1);
        }
        if(!is_wall(cur_y, cur_x + 1))
        {
            visit_tile(curr_path, cur_y, cur_x, 0, 1);
        }
    }
    return best_path;
}

void Pinky::BFSChase()
{
    int pinky_x = static_cast<int>(std::floor(pos().x() / size));
    int pinky_y = static_cast<int>(std::floor(pos().y() / size));
    path_t best_path = BFS(pinky_x, pinky_y);
    if(best_path.size() < 2) return;
    auto [dy, dx] = std::make_pair(best_path[1].first - pinky_y, best_path[1].second - pinky_x);
    switch (dy)
    {
    case -1:
        setDir(UP);
        break;
    case 1:
        setDir(DOWN);
    }
    switch (dx)
    {
    case -1:
        setDir(LEFT);
        break;
    case 1:
        setDir(RIGHT);
        break;
    }

}

void Pinky::move()
{
    updatePacmanPos();
    BFSChase();
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    setPos(x + x_v, y + y_v);
}
