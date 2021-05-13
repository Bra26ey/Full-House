#include "gamefragment.h"
#include "mainwindow.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QSound>
#include <QVector>
#include <QIntValidator>


int base_card_coefficient = 280;
int card_move_coefficient = 90;

int min_base_card_coefficient = 166;
int min_card_move_coefficient = 80;

using namespace screens;
GameFragment::GameFragment() : num_players(0) {
    mPlayTable = new PlayTable;
    mDealerLogo = new DealerLogo;

    mChips = new Chips;
    mChips->setParent(mPlayTable);

    mWinLabel = new WinLabel;
    mWinLabel->setParent(mPlayTable);

    mTurnIndicator = new TurnSpark;
    mTurnIndicator->hide();

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    mainVLayout->setAlignment(Qt::AlignCenter);

    mainVLayout->addWidget(mPlayTable);


    BetButton = new QPushButton("Bet");
    BetButton->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
    connect(BetButton, &QPushButton::clicked, this, &GameFragment::onBetPressed);

    CallButton = new QPushButton("Call");
    CallButton->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
    connect(CallButton, &QPushButton::clicked, this, &GameFragment::onCallPressed);

    RaiseButton = new QPushButton("Raise");
    RaiseButton->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
    connect(RaiseButton, &QPushButton::clicked, this, &GameFragment::onRaisePressed);

    FoldButton = new QPushButton("Fold");
    FoldButton->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
    connect(FoldButton, &QPushButton::clicked, this, &GameFragment::onFoldPressed);


    CheckButton = new QPushButton("Check");
    CheckButton->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
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

    ActionButtons.append(BetButton);
    ActionButtons.append(CallButton);
    ActionButtons.append(RaiseButton);
    ActionButtons.append(FoldButton);
    ActionButtons.append(CheckButton);
    foreach (auto btn, ActionButtons) {
        mainHLayout->addWidget(btn);
        btn->hide();
    }
    mainVLayout->addLayout(mainHLayout);

    QHBoxLayout *SliderLayout = new QHBoxLayout;

    SliderLayout->addWidget(BetSlider);
    SliderLayout->addWidget(BetValue);

    mainVLayout->addLayout(SliderLayout);
    BetSlider->hide();
    BetValue->hide();

    QHBoxLayout *extraHLayout = new QHBoxLayout;

    SettingsButton = new QPushButton("Settings");
    SettingsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SettingsButton, &QPushButton::clicked, this, &GameFragment::onSettingsPressed);

    StartGameButton = new QPushButton("StartGame");
    StartGameButton->setStyleSheet("color:#242424;font-size:24px");
    connect(StartGameButton, &QPushButton::clicked, this, &GameFragment::onStartPressed);

    LeaveButton = new QPushButton("Exit");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &GameFragment::onLeavePressed);


    extraHLayout->addWidget(SettingsButton);
    extraHLayout->addWidget(StartGameButton);
    extraHLayout->addWidget(LeaveButton);

    mainVLayout->addLayout(extraHLayout);
    this->setLayout(mainVLayout);


    DrawMainPlayer();
    // кайнда дебаг



    DrawPlayer(1, "Cartman", 5000);
    DrawPlayer(2, "Kenny", 2000);
    DrawPlayer(3, "Stan", 6000);
    DrawPlayer(4, "Wendy", 1000);
    DrawPlayer(5, "Dougie", 500);


    mPlayer->GiveCards(14,2, 14,3);
    mPlayer->FlipCards();

    mOtherPlayers[0]->GiveCards(4,3,2,3);
    mOtherPlayers[1]->GiveCards(4,3,2,3);
    mOtherPlayers[0]->FlipCards();
    mOtherPlayers[2]->GiveCards(4,3,2,3);
    mOtherPlayers[3]->GiveCards(4,3,2,3);
    mOtherPlayers[4]->GiveCards(4,3,2,3);
    mOtherPlayers[4]->FlipCards();


    mOtherPlayers[3]->setBet(400);
    mOtherPlayers[2]->DiscardCards();
    mOtherPlayers[2]->setFold();
    mOtherPlayers[3]->setRaise();
    mOtherPlayers[3]->ClearStatus();
    mOtherPlayers[1]->setCheck();
    mOtherPlayers[4]->setRaise();


    CurrentTurn(mPlayer);
    MakeDealer(4);
    DisplayWinner(mPlayer);

    AddCardToTable(4, 2, true);
    AddCardToTable(5, 2, true);
    AddCardToTable(7, 2, true);
    AddCardToTable(14, 3, true);
    AddCardToTable(2, 1, true);
}

GameFragment::~GameFragment() {
    delete mPlayer;
    delete mPlayTable;

    delete BetButton;
    delete RaiseButton;
    delete FoldButton;
    delete CheckButton;
    delete BetSlider;
    delete BetValue;

    delete LeaveButton;
    delete SettingsButton;
    delete StartGameButton;
}

void GameFragment::setval() {
    auto a = BetSlider->value();
    QString text = QString::number(a);
    BetValue->setText(text);
}

void GameFragment::onBetPressed() {
    mPlayer->setBet(BetValue->text().toUInt());
    mChips->AddToBank(BetValue->text().toUInt());
//    BlockActions();

}

void GameFragment::onCallPressed() {
    ActionButtons[0]->hide();
    ActionButtons[1]->hide();
    ActionButtons[1]->show();
    mPlayer->setCall();
}

void GameFragment::onCheckPressed() {
    mPlayer->setCheck();
    FlipAllCards();
    //BlockActions();
}

void GameFragment::onRaisePressed() {
    mPlayer->setRaise();
    DisplayWinner(mOtherPlayers[2]);
    //BlockActions();
}

void GameFragment::onFoldPressed() {
    mPlayer->setFold();
    DeleteWinnerDisplay();
    //BlockActions();
}

void GameFragment::onLeavePressed() {
    Client->LeaveRoom();
    back();
}
void GameFragment::onStartPressed() {
    foreach (auto btn, ActionButtons) {
        btn->show();
    }
    BetSlider->show();
    BetValue->show();
    StartGameButton->hide();
}
void GameFragment::onSettingsPressed() {
    navigateTo(SETTINGS_TAG);
}

void GameFragment::DrawPlayer(size_t player_id, std::string nickname, size_t total_money) {
    qDebug() << player_id;
    OtherPlayer* player = new OtherPlayer(player_id, nickname, total_money);
    mOtherPlayers.append(player);
    mOtherPlayers[num_players]->setParent(mPlayTable);
    switch (player_id) {
        case 1: {
        mOtherPlayers[num_players]->setGeometry(firstpos);
        mOtherPlayers[num_players]->SetPosition(firstpos);
        break;
        }
        case 2: {
        mOtherPlayers[num_players]->setGeometry(secondpos);
        mOtherPlayers[num_players]->SetPosition(secondpos);
        break;
        }
        case 3: {
        mOtherPlayers[num_players]->setGeometry(thirdpos);
        mOtherPlayers[num_players]->SetPosition(thirdpos);
        break;
        }
        case 4: {
        mOtherPlayers[num_players]->setGeometry(fouthpos);
        mOtherPlayers[num_players]->SetPosition(fouthpos);
        break;
        }
        case 5: {
        mOtherPlayers[num_players]->setGeometry(fifthpos);
        mOtherPlayers[num_players]->SetPosition(fifthpos);
        break;
        }
    }
    num_players++;
}

void GameFragment::DrawMainPlayer() {
    mPlayer = new Player("Sample Text", 9999);
    mPlayer->setParent(mPlayTable);
    mPlayer->setGeometry(mainplayerpos);
    mPlayer->SetPosition(mainplayerpos);
}

void GameFragment::GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->GiveCards(value1, suit1, value2, suit2);
    } else {
        mPlayer->GiveCards(value1, suit1, value2, suit2);
    }
}

void GameFragment::MakeDealer(size_t player_id) {
    if (player_id > 0) {
        mDealerLogo->setParent(mOtherPlayers[player_id - 1]);
        mDealerLogo->setGeometry(10, 20, 200, 200);
    } else {
        mDealerLogo->setParent(mPlayer);
        mDealerLogo->setGeometry(-20, -10, 200, 200);
    }
}

void GameFragment::FlipAllCards() {
    foreach(auto player, mOtherPlayers) {
        if (!player->GetCardSide())
        player->FlipCards();
    }
}

void GameFragment::BlockActions() {
    foreach (auto btn, ActionButtons) {
        btn->blockSignals(true);
        btn->setStyleSheet("background:rgb(245, 65, 0);color:#242424;font-size:24px");
    }
}

void GameFragment::UnBlockActions() {
    foreach (auto btn, ActionButtons) {
        btn->blockSignals(false);
        btn->setStyleSheet("background:rgb(74, 212, 104);color:#242424;font-size:24px");
    }
}

void GameFragment::DisplayWinner(OtherPlayer *winner) {
    winner->AddMoney(mChips->GetBank());

    QString text = "Winner is " + winner->GetName() + ". Won " + QString::number(mChips->GetBank()) + "$";
    mChips->Wipe();
    mWinLabel->setText(text);
    mWinLabel->setAlignment(Qt::AlignCenter);

    mWinLabel->show();
}

void GameFragment::CurrentTurn(OtherPlayer *player) {
    mTurnIndicator->setParent(player);
    if (player == mPlayer) {
        mTurnIndicator->setGeometry(85, -25, 300, 300);
    } else {
        mTurnIndicator->setGeometry(90, -80, 300, 300);
    }
    mTurnIndicator->show();
}

void GameFragment::DeleteWinnerDisplay() {
    mWinLabel->hide();
}

void GameFragment::AddCardToTable(size_t value, size_t suit, bool upsided) {
    auto card = new Card(value, suit, upsided);
    card->setParent(mPlayTable);
    CardOnTable.push_back(card);
}

void GameFragment::DeleteAllCardsFromTable() {
    CardOnTable.clear();
}

void GameFragment::RedrawPlayer(OtherPlayer* player) {
    auto pos = player->GetPos();
    player->setGeometry(pos);
    player->SetPosition(pos);
}


void GameFragment::resizeEvent(QResizeEvent *event) {
    mPlayTable->Resize(this->size());
    foreach(auto player, mOtherPlayers) {
        player->Resize(this->size());
        if(player->HasCards) {
            player->GetCard().first->Resize(this->size());
            player->GetCard().second->Resize(this->size());
        }
        RedrawPlayer(player);
    }
    mPlayer->Resize(this->size());
    if (mPlayer->HasCards) {
        mPlayer->GetCard().first->Resize(this->size());
        mPlayer->GetCard().second->Resize(this->size());
    }
    RedrawPlayer(mPlayer);
    int i = 0;
    foreach(auto card, CardOnTable) {
        card->ResizeOnTable(this->size(), i);
        i++;
    }
    mChips->Resize(this->size());
    mWinLabel->Resize(this->size());
}

