#ifndef GAMEOVERPAGE_H
#define GAMEOVERPAGE_H

#include <QWidget>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class GameOverPage; }
QT_END_NAMESPACE

class GameOverPage : public QWidget
{
    Q_OBJECT
private:
    QStackedWidget* layout_ref;
    Ui::GameOverPage* ui;
    void connectButtons();
private slots:
    void quit();
    void backToMenu();
public slots:
    void displayScore();
public:
    GameOverPage(QWidget *parent = nullptr, QStackedWidget* layout_ref = nullptr);
    ~GameOverPage();
};

#endif // GAMEOVERPAGE_H
