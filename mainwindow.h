#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

class GamePage;
class SettingsPage;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void startButtonClicked();
    void quitButtonClicked();
    void settingsButtonClicked();

    GamePage* game_page;
    SettingsPage* settings_page;
    void connectButtons();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
