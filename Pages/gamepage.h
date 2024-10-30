#ifndef GAMEPAGE_H
#define GAMEPAGE_H
#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QStackedWidget>
#include <vector>
#include "../Entities/pacman.h"
#include "../Entities/ghost.h"
#include "../MapElements/collectable.h"
#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define NO_OF_GHOSTS 4

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class GamePage; }
QT_END_NAMESPACE

class GamePage : public QWidget
{
    Q_OBJECT
private:
    QStackedWidget* layout_ref;
    Ui::GamePage *ui;
    QGraphicsScene *scene;
    QTimer * player_timer;
    QTimer * power_up_timer;

    int current_level;
    int mapGrid[MAP_HEIGHT][MAP_WIDTH];
    Pacman *pacman;
    Ghost *ghosts[NO_OF_GHOSTS];

    std::vector<Collectable*> collectables;

    void connectTimers();
    void powerUpMode();
    /*To be moved to MapLoader class*/ void drawMapGrid();
    /*To be moved to MapLoader class*/ void loadLevel(int lvl_number);
    /*To be moved to MapLoader class*/ void initializeGrid(const QJsonArray &jsonArr);
    void gameOver();
    void newLifeRestart();
private slots:
    void endPowerUpMode();
    void handlePacmanCollision();
    void collectCollectables(const QList<QGraphicsItem*> &collisions);
    void ghostCollisions(const QList<QGraphicsItem*> &collisions);
    void updateScore();
protected:
    void keyPressEvent(QKeyEvent *event) override;
public slots:
    void run();
    void backToGame();
public:
    void resetGame();
    GamePage(QWidget *parent = nullptr, QStackedWidget* ref = nullptr);
    ~GamePage();
};

#endif // GAMEPAGE_H
