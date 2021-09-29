#include "settingsfragment.h"
#include "mediaplayer.h"
#include "client_impl.h"

#include <QSound>
#include <QLabel>

SettingsFragment::SettingsFragment() : EasterEgg(0) {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;
    QVBoxLayout *buttonContainer = new QVBoxLayout;

    QString text;
    if (player->isMuted())
    text = "Unmute";
    else text = "Mute";
    MuteButton = new QPushButton(text);
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

    Nickname = new QLabel("Nickname: " + QString::fromUtf8(globalInfo::Nickname.c_str()));
    Nickname->setAlignment(Qt::AlignHCenter);
    Nickname->setStyleSheet("margin-top:25px");


    Money = new QLabel("Money:" + QString::number(globalInfo::Balance));
    Money->setAlignment(Qt::AlignHCenter);
    Money->setStyleSheet("margin-top:25px;margin-bottom:20px");

    buttonContainer->addWidget(MuteButton);
    buttonContainer->addWidget(title);
    buttonContainer->addWidget(VolumeSlider);
    buttonContainer->addWidget(Nickname);
    buttonContainer->addWidget(Money);
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
    QSound::play(":/music/click");
    back();
}

void SettingsFragment::MutePressed() {
    if (player->isMuted()) {
        player->setMuted(false);
        MuteButton->setText("Mute");
        EasterEgg++;
    } else {
        player->setMuted(true);
        MuteButton->setText("Unmute");
    }
    if (EasterEgg == 10) {
        playlist->addMedia(QUrl("qrc:/music/Media/EasterEgg.mp3"));
        playlist->next();
    } if (EasterEgg == 11) {
        playlist->addMedia(QUrl("qrc:/music/Media/BackGround.mp3"));
        playlist->next();
    }
}
