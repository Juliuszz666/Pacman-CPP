#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QMap>
#include <QStack>
#include <QPushButton>
#include <QMessageBox>
#include <QSet>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class PacMan;
class Ghost;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define KEY_ESCAPE 16777216
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

enum keyActions
{
    MVUP,
    MVLEFT,
    MVDOWN,
    MVRIGHT,
    PAUSE,
    SETTINGS
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void settingsPageKeysHandling(int key);
    void gamePageKeysHandling(int key);

    void bindKey();

    void startButtonClicked();
    void quitButtonClicked();
    void settingsButtonClicked();
    void backButtonClicked();
    void setUpButtonActions();
    //void gameLoop();

private:
    int currentLevel;
    int mapGrid[MAP_HEIGHT][MAP_WIDTH];
    QMap<QPushButton*, keyActions> buttonActions;
    QMap<keyActions, int> keyBindings;
    QStack<int> pageIndexStack;
    const QMap<keyActions, int> defaultBindings =
    {
        {MVUP, 'W'},
        {MVLEFT, 'A'},
        {MVDOWN, 'S'},
        {MVRIGHT, 'D'},
        {SETTINGS, KEY_ESCAPE},
        {PAUSE, 'P'}
    };

    void checkDuplicates();
    void connectButtons();
    void pushPage(int index);
    void popPage();
    void initializeGrid(const QJsonArray &jsonArr);

    void loadLevel(int lvl_number);

    void drawMapGrid();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    PacMan *pacman;
    Ghost **ghosts;
    QTimer *timer;
    QPushButton* currentButton;
};

#endif // MAINWINDOW_H
