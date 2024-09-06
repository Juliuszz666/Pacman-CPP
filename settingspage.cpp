#include "settingspage.h"
#include "./ui_settingspage.h"
#include <QMessageBox>

SettingsPage::SettingsPage(QWidget *parent, QStackedWidget* ref) :
    QWidget(parent),
    ui(new Ui::SettingsPage),
    currentButton(nullptr),
    layout_ref(ref)
{
    ui->setupUi(this);
    setUpButtonActions();
    setUpMusic();
    connectButtons();
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::checkDuplicates()
{
    QSet<int> checkSet;
    for(auto element = Shared::keyBindings.constBegin(); element != Shared::keyBindings.constEnd(); ++element)
    {
        if(checkSet.contains(element.value()))
        {
            Shared::keyBindings = Shared::defaultBindings;
            setUpButtonActions();
            QMessageBox::critical(nullptr, "Error", "Error with key binding, restored to default");
            currentButton = nullptr;
            return;
        }
        else
        {
            checkSet.insert(element.value());
        }
    }
}

void SettingsPage::setUpMusic()
{
    Shared::music_player->setAudioOutput(Shared::audio_output.get());
    Shared::music_player->setSource(QUrl("qrc:/mp3/testmusic.mp3"));
    Shared::music_player->play();
}

void SettingsPage::changeVolume()
{
    qreal vol = QAudio::convertVolume(ui->volumeSlider->value() / qreal(100.0), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    Shared::audio_output->setVolume(vol);
}

void SettingsPage::setUpButtonActions()
{
    buttonActions.insert(ui->mvDownBinder, MVDOWN);
    ui->mvDownBinder->setText(QKeySequence(Shared::keyBindings[MVDOWN]).toString());
    buttonActions.insert(ui->mvUpBinder, MVUP);
    ui->mvUpBinder->setText(QKeySequence(Shared::keyBindings[MVUP]).toString());
    buttonActions.insert(ui->mvLeftBinder, MVLEFT);
    ui->mvLeftBinder->setText(QKeySequence(Shared::keyBindings[MVLEFT]).toString());
    buttonActions.insert(ui->mvRightBinder, MVRIGHT);
    ui->mvRightBinder->setText(QKeySequence(Shared::keyBindings[MVRIGHT]).toString());
    buttonActions.insert(ui->pauseBinder, PAUSE);
    ui->pauseBinder->setText(QKeySequence(Shared::keyBindings[PAUSE]).toString());
    buttonActions.insert(ui->settingsBinder, SETTINGS);
    ui->settingsBinder->setText(QKeySequence(Shared::keyBindings[SETTINGS]).toString());
    /*VOLUME SLIDER*/
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
}

void SettingsPage::keyPressEvent(QKeyEvent * event)
{
    int key = event->key();
    if(currentButton)
    {
        keyActions action = buttonActions[currentButton];
        for(auto it = Shared::keyBindings.constBegin(); it != Shared::keyBindings.constEnd(); ++it)
        {
            if(it.value() == key)
            {
                Shared::keyBindings[action] = Shared::defaultBindings[action];
                currentButton->setText(QKeySequence(Shared::defaultBindings[action]).toString());
                checkDuplicates();
                return;
            }
        }
        Shared::keyBindings[action] = key;
        currentButton->setText(QKeySequence(key).toString());
        currentButton = nullptr;
    }
    else
    {
        if(key == Shared::keyBindings[SETTINGS])
        {
            Shared::pageIndexStack.pop();
            if(Shared::pageIndexStack.top() == GAME_PAGE)
            {
                layout_ref->setCurrentIndex(GAME_PAGE);
            }
            else
            {
                Shared::pageIndexStack.push(SETTINGS_PAGE);
                layout_ref->setCurrentIndex(SETTINGS_PAGE);
            }
        }
    }
}

void SettingsPage::bindKey()
{
    if (currentButton)
    {
        keyActions action = buttonActions[currentButton];
        currentButton->setText(QKeySequence(Shared::keyBindings[action]).toString());
    }
    currentButton = qobject_cast<QPushButton*>(sender());
    if(currentButton)
    {
        currentButton->setText("Press key...");
    }
}

void SettingsPage::connectButtons()
{
    /* BACK BUTTON */
    connect(ui->backButton, &QPushButton::clicked, this, &SettingsPage::backButtonClicked);
    /* BINDING BUTTONS */
    connect(ui->mvDownBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    connect(ui->mvUpBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    connect(ui->mvLeftBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    connect(ui->mvRightBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    connect(ui->pauseBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    connect(ui->settingsBinder, &QPushButton::clicked, this, &SettingsPage::bindKey);
    /* VOLUME SLIDER */
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &SettingsPage::changeVolume);
}

void SettingsPage::backButtonClicked()
{
    Shared::pageIndexStack.clear();
    Shared::pageIndexStack.push(WELCOME_PAGE);
    layout_ref->setCurrentIndex(WELCOME_PAGE);
}
