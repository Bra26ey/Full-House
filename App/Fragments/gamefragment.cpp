#include "gamefragment.h"
#include "mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QSound>
#include <QVector>
#include <QIntValidator>


int base_card_coefficient = -50;
int card_move_coefficient = 90;

using namespace screens;
GameFragment::GameFragment() : num_players(0) {
    mPlayTable = new PlayTable;
    mDealerLogo = new DealerLogo;

    bool up;
    for(size_t i = 0; i < 5; ++i) {
        i < 3 ? up = true : up = false;
        auto card = new Card(2 * i + 1,1 * i + 1, up);
        CardOnTable.push_back(card);
        CardOnTable[i]->setParent(mPlayTable);
        CardOnTable[i]->setGeometry(mPlayTable->width()/2 + base_card_coefficient + i * card_move_coefficient, mPlayTable->height()/2, mPlayTable->width()/2 + 200, mPlayTable->height()/2 + 50);
    }
    CardOnTable[3]->Flip();




    QVBoxLayout *mainVLayout = new QVBoxLayout;
    mainVLayout->setAlignment(Qt::AlignCenter);

    mainVLayout->addWidget(mPlayTable);


    BetButton = new QPushButton("Bet");
    BetButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BetButton, &QPushButton::clicked, this, &GameFragment::onBetPressed);

    RaiseButton = new QPushButton("Raise");
    RaiseButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RaiseButton, &QPushButton::clicked, this, &GameFragment::onRaisePressed);

    FoldButton = new QPushButton("Fold");
    FoldButton->setStyleSheet("color:#242424;font-size:24px");
    connect(FoldButton, &QPushButton::clicked, this, &GameFragment::onFoldPressed);


    CheckButton = new QPushButton("Check");
    CheckButton->setStyleSheet("color:#242424;font-size:24px");
    connect(CheckButton, &QPushButton::clicked, this, &GameFragment::onCheckPressed);

    BetSlider = new QSlider(Qt::Horizontal); // слайдер ставки
    BetSlider->setFocusPolicy(Qt::NoFocus);
    BetSlider->setMaximumWidth(500);
    BetSlider->setStyleSheet("color:#242424;margin-top:50px");
    BetSlider->setRange(1, 1000);
    BetSlider->setTickInterval(1);
    int minbet = 10;
    BetSlider->setValue(minbet);/*minbet*/

    QString value = QString::number(minbet);
    BetValue = new QLineEdit(value, this); /*minbet*/ // ввод размера ставки
    BetValue->setMaximumWidth(70);
    BetValue->setValidator(new QIntValidator(1, 1000, this));  // ограничиваем ставку
    BetValue->setStyleSheet("font-size:20px");

    connect(BetSlider, &QSlider::valueChanged, this, &GameFragment::setval);  //  напрямую нельзя связать qlinedit и slider

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

    SettingsButton = new QPushButton("Settings");
    SettingsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SettingsButton, &QPushButton::clicked, this, &GameFragment::onSettingsPressed);

    LeaveButton = new QPushButton("Exit");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &GameFragment::onLeavePressed);

    extraHLayout->addWidget(SettingsButton);
    extraHLayout->addWidget(LeaveButton);

    mainVLayout->addLayout(extraHLayout);
    this->setLayout(mainVLayout);

    // кайнда дебаг

    DrawPlayer(firstpos, 1);
    DrawPlayer(secondpos, 2);
    DrawPlayer(thirdpos, 3);
    DrawPlayer(fouthpos, 4);
    DrawPlayer(fifthpos, 5);

    OtherPlayers[0]->GiveCards(4,4,2,3);
    OtherPlayers[1]->GiveCards(4,4,2,3);
    OtherPlayers[0]->FlipCards();
    OtherPlayers[2]->GiveCards(4,4,2,3);
    OtherPlayers[3]->GiveCards(4,4,2,3);
    OtherPlayers[4]->GiveCards(4,4,2,3);
    OtherPlayers[4]->FlipCards();

    MakeDealer(1);
}

GameFragment::~GameFragment() {
    delete mPlayer;
    delete mPlayTable;
    delete mDealerLogo;

    delete BetButton;
    delete RaiseButton;
    delete FoldButton;
    delete CheckButton;
    delete BetSlider;
    delete BetValue;

    delete LeaveButton;
    delete SettingsButton;
}

void GameFragment::setval() {
    auto a = BetSlider->value();
    QString text = QString::number(a);
    BetValue->setText(text);
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

void GameFragment::DrawPlayer(QRect pos, size_t player_id) {
    OtherPlayer* player = new OtherPlayer(player_id);
    OtherPlayers.append(player);
    OtherPlayers[num_players]->setParent(mPlayTable);
    OtherPlayers[num_players]->setGeometry(pos);
    OtherPlayers[num_players]->SetPosition(pos);
    num_players++;
}

void GameFragment::MakeDealer(size_t player_id) {
    if (player_id > 0)
    mDealerLogo->setParent(OtherPlayers[player_id - 1]);
    //else setParentPlayer
    mDealerLogo->setGeometry(20, 30, 150, 150);
}
