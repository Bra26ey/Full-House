#include "gamefragment.h"
#include "mainwindow.h"

#include <QImage>
#include <QPixmap>

using namespace screens;
GameFragment::GameFragment() {
    QImage * mypix = new QImage("/home/zennoma/Full-House/App/Media/table.png");
    QImage table = mypix->scaled(640, 480, Qt::IgnoreAspectRatio);
    QVBoxLayout *mainVLayout = new QVBoxLayout;

    PlayTable = new QLabel(this);

    PlayTable->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
    PlayTable->resize(640, 480);

    mainVLayout->addWidget(PlayTable);
    mainVLayout->setAlignment(Qt::AlignCenter);



    BetButton = new QPushButton("Сделать ставку");
    BetButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BetButton, &QPushButton::clicked, this, &GameFragment::onBetPressed);

    RaiseButton = new QPushButton("Сделать Рейз?");
    RaiseButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RaiseButton, &QPushButton::clicked, this, &GameFragment::onRaisePressed);

    FoldButton = new QPushButton("Фолд");
    FoldButton->setStyleSheet("color:#242424;font-size:24px");
    connect(FoldButton, &QPushButton::clicked, this, &GameFragment::onFoldPressed);


    CheckButton = new QPushButton("Чек");
    CheckButton->setStyleSheet("color:#242424;font-size:24px");
    connect(CheckButton, &QPushButton::clicked, this, &GameFragment::onCheckPressed);

    BetSlider = new QSlider(Qt::Horizontal);
    BetSlider->setFocusPolicy(Qt::NoFocus);
    BetSlider->setMaximumWidth(500);
    BetSlider->setStyleSheet("color:#242424;margin-top:50px");
    BetSlider->setRange(1, 1000);
    BetSlider->setTickInterval(1);/*minbet*/
    BetSlider->setValue(1);


    BetValue = new QLabel("1", this);
    BetSlider->setStyleSheet("color:#242424;font-size:24px;margin-top:50px");
    BetValue->setMaximumWidth(50);

    connect(BetSlider, &QSlider::valueChanged, BetValue, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));

    QHBoxLayout *mainHLayout = new QHBoxLayout;

    mainHLayout->addWidget(BetButton);
    mainHLayout->addWidget(RaiseButton);
    mainHLayout->addWidget(FoldButton);
    mainHLayout->addWidget(CheckButton);

    mainVLayout->addLayout(mainHLayout);

    QHBoxLayout *SliderLayout = new QHBoxLayout;

    SliderLayout->addWidget(BetSlider);
    SliderLayout->addWidget(BetValue);

    mainVLayout->addLayout(SliderLayout);

    QHBoxLayout *extraHLayout = new QHBoxLayout;

    SettingsButton = new QPushButton("Настройки");
    SettingsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SettingsButton, &QPushButton::clicked, this, &GameFragment::onSettingsPressed);

    LeaveButton = new QPushButton("Выход");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &GameFragment::onLeavePressed);

    extraHLayout->addWidget(SettingsButton);
    extraHLayout->addWidget(LeaveButton);

    mainVLayout->addLayout(extraHLayout);
    this->setLayout(mainVLayout);
}

GameFragment::~GameFragment() {
    delete Player;
    delete PlayTable;

    delete BetButton;
    delete RaiseButton;
    delete FoldButton;
    delete CheckButton;
    delete BetSlider;
    delete BetValue;

    delete LeaveButton;
    delete SettingsButton;
}


void GameFragment::onBetPressed() {

}

void GameFragment::onCheckPressed() {

}

void GameFragment::onRaisePressed() {

}

void GameFragment::onFoldPressed() {

}

void GameFragment::onLeavePressed() {
    // disconnect
    navigateTo(SEARCH_TAG);
}

void GameFragment::onSettingsPressed() {
    navigateTo(SETTINGS_TAG);
}

void GameFragment::ChangedSlider() {

}
