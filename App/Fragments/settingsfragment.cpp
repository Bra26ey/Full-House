#include "settingsfragment.h"
#include "mediaplayer.h"

#include <QLabel>

SettingsFragment::SettingsFragment() {
    qDebug("Settings create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;



    QVBoxLayout *buttonContainer = new QVBoxLayout;

    MuteButton = new QPushButton("Mute");
    MuteButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MuteButton, &QPushButton::clicked, this, &SettingsFragment::MutePressed);

    MenuButton = new QPushButton("Back");
    MenuButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MenuButton, &QPushButton::clicked, this, &SettingsFragment::onMenuPressed);

    VolumeSlider = new QSlider(Qt::Horizontal, this);
    VolumeSlider->setMaximumWidth(500);
    VolumeSlider->setStyleSheet("color:#242424;margin-top:20px;margin-bottom:50px");
    VolumeSlider->setRange(0, 100);
    VolumeSlider->setValue(player->volume());
    connect(VolumeSlider, &QSlider::valueChanged, this, &SettingsFragment::ChangedSlider);


    QLabel *title = new QLabel("Volume", VolumeSlider);
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("margin-top:50px");

    buttonContainer->addWidget(MuteButton);
    buttonContainer->addWidget(title);
    buttonContainer->addWidget(VolumeSlider);
    buttonContainer->addWidget(MenuButton);



    buttonContainer->addWidget(MenuButton);
    startMainLayout->addLayout(buttonContainer);
    startContent->setContentsMargins(46,46,46,46);

    centerConainer->setStyleSheet("color:#242424;font-size:24px");
    centerConainer->setLayout(startContent);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerConainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}

SettingsFragment::~SettingsFragment() {
    delete MuteButton;
    delete MenuButton;
    delete VolumeSlider;
}

void SettingsFragment::ChangedSlider() {
    player->setVolume(VolumeSlider->value());
}

void SettingsFragment::onMenuPressed() {
    back();
}

void SettingsFragment::MutePressed() {
    if (player->isMuted()) {
        player->setMuted(false);
        MuteButton->setText("Mute");
    } else {
        player->setMuted(true);
        MuteButton->setText("Unmute");
    }
}
