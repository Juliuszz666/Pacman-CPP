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
#include "../shared.h"

constexpr int NO_OF_GHOSTS = 4;

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
    int max_level;
    int mapGrid[MAP_HEIGHT][MAP_WIDTH];
    Pacman *pacman;
    Ghost *ghosts[NO_OF_GHOSTS];

    std::vector<Collectable*> collectables;

    void newLevel();
    void connectTimers();
    void powerUpMode();
    void drawMapGrid();
    void newLifeRestart();
    void gameOver();
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
signals:
    void gameOverSignal();
public:
    void resetGame();
    GamePage(QWidget *parent = nullptr, QStackedWidget* ref = nullptr);
    ~GamePage();
};

#endif // GAMEPAGE_H
