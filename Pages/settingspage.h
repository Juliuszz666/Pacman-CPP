#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H
#include "../shared.h"
#include <QWidget>
#include <QKeyEvent>
#include <QPushButton>
#include <QStackedWidget>
#include <QtMultimedia>

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsPage; }
QT_END_NAMESPACE

class SettingsPage : public QWidget
{
    Q_OBJECT
private:
    QStackedWidget* layout_ref;
    Ui::SettingsPage *ui;
    QMap<QPushButton*, keyActions> buttonActions;
    QPushButton* currentButton;
    std::unique_ptr<QAudioOutput> audio_output;
    std::unique_ptr<QMediaPlayer> music_player;

    void checkDuplicates();
    void changeVolume();
    void setUpButtonActions();
    void setUpMusic();
    void connectButtons();
    void backButtonClicked();
    void bindKey();

signals:
    void backToGameSignal();
    void backButtonClickedSignal();

protected:
    void keyPressEvent(QKeyEvent *event) override;
public:
    SettingsPage(QWidget *parent = nullptr, QStackedWidget* ref = nullptr);
    ~SettingsPage();
};


#endif // SETTINGSPAGE_H
