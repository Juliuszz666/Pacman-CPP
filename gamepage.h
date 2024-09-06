#ifndef GAMEPAGE_H
#define GAMEPAGE_H
#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QStackedWidget>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20

class MainWindow;

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

    int current_level;
    int mapGrid[MAP_HEIGHT][MAP_WIDTH];

    void drawMapGrid();
    void loadLevel(int lvl_number);
    void initializeGrid(const QJsonArray &jsonArr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
public:
    GamePage(QWidget *parent = nullptr, QStackedWidget* ref = nullptr);
    ~GamePage();
};

#endif // GAMEPAGE_H
