#include "gamefragment.h"
#include "mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QSound>

using namespace screens;
GameFragment::GameFragment() {


    playtable = new PlayTable;

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(playtable);
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
    BetSlider->setTickInterval(1);
    int minbet = 10;
    BetSlider->setValue(minbet);/*minbet*/

    QString value = QString::number(minbet);
    BetValue = new QLabel(value,this); /*minbet*/
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
    delete playtable;

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

