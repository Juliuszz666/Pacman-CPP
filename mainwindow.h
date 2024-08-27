#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QMap>
#include <QPushButton>
class PacMan;
class Ghost;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define KEY_ESCAPE 16777216

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
    void bindKey();

    void startButtonClicked();
    void quitButtonClicked();
    void settingsButtonClicked();
    void backButtonClicked();
    void setUpButtonActions();
    //void gameLoop();

private:
    QMap<QPushButton*, keyActions> buttonActions;
    QMap<keyActions, int> keyBindings;
    const QMap<keyActions, int> defaultBindings =
    {
        {MVUP, 'W'},
        {MVLEFT, 'A'},
        {MVDOWN, 'S'},
        {MVRIGHT, 'D'},
        {SETTINGS, KEY_ESCAPE},
        {PAUSE, 'P'}
    };

    void connectButtons();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    PacMan *pacman;
    Ghost **ghosts;
    QTimer *timer;
    QPushButton* currentButton;
};

#endif // MAINWINDOW_H
