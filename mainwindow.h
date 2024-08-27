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
    //void gameLoop();

private:
    QMap<QPushButton*, QString> buttonActions;
    QMap<QString, int> keyBindings;

    void connectButtons();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    PacMan *pacman;
    Ghost **ghosts;
    QTimer *timer;
    QPushButton* currentButton;
};

#endif // MAINWINDOW_H
