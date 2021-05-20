#include "gamefragment.h"
#include "mainwindow.h"

#include <QDebug>
#include <QSound>
#include <QVector>
#include <QIntValidator>


int base_card_coefficient = 280;
int card_move_coefficient = 90;

int min_base_card_coefficient = 166;
int min_card_move_coefficient = 80;

using namespace screens;
GameFragment::GameFragment() : mMinbet(1), mMaxbet(10), mOtherPlayers(5, nullptr) {
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



    BetSlider->setRange(mMinbet, mMaxbet);
    BetSlider->setTickInterval(1);
    QString value = QString::number(mMinbet);
    BetValue = new QLineEdit(value, this);
    BetValue->setMaximumWidth(70);
    BetValue->setValidator(new QIntValidator(mMinbet, mMaxbet, this));  // ограничиваем ставку
    BetValue->setStyleSheet("font-size:20px");

    connect(BetSlider, &QSlider::valueChanged, this, &GameFragment::setval);  //  напрямую нельзя связать qlinedit и slider

    QHBoxLayout *mainHLayout = new QHBoxLayout;

    ActionButtons.append(FoldButton);
    ActionButtons.append(CallButton);
    ActionButtons.append(RaiseButton);
    ActionButtons.append(CheckButton);
    ActionButtons.append(BetButton);

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

    is_active = false;


    DrawMainPlayer();
    // кайнда дебаг
    // SetMinBet(10);
    // SetMaxBet(1000);


    // DrawPlayer(5, "Cartman", 5000);
    // DrawPlayer(1, "Kenny", 2000);
    // DrawPlayer(3, "Stan", 6000);
    // DrawPlayer(2, "Wendy", 1000);
    // DrawPlayer(4, "Dougie", 500);


    // GiveCards(0, 14,2, 14,3);
    // FlipCards(0);

    // GiveCards(1, 4,3,2,1);
    // GiveCards(2, 14,3,2,0);
    // GiveCards(3, 12,3,2,3);
    // GiveCards(4, 13,3,2,2);
    // GiveCards(5, 11,3,2,1);
    // FlipCards(1);


    // SetBet(4, 400);
    // SetFold(3);

    // CurrentTurn(0);
    // MakeDealer(4);
    // DisplayWinner(0);

    // AddCardToTable(4, 2, true);
    // AddCardToTable(5, 2, true);
    // AddCardToTable(7, 2, true);
    // AddCardToTable(14, 3, true);
    // AddCardToTable(2, 1, true);

    // DeletePlayer(1);
    // DrawPlayer(1, "Cartman2", 5000);
    // DeletePlayer(2);
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
    QSound::play(":/music/click");
    auto bet = BetValue->text().toInt();
    if (bet > mMaxbet) {
        bet = mMaxbet;
        BetValue->setText(QString::number(bet));
    }
    if (bet < mMinbet) {
        bet = mMinbet;
        BetValue->setText(QString::number(bet));
    }
    mPlayer->setBet(bet);
    mChips->AddToBank(bet);
    Client->GameRaise(BetValue->text().toUInt());
    BlockActions();
}

void GameFragment::onCallPressed() {
    QSound::play(":/music/click");
    SetCall(0);
    Client->GameCall();
    BlockActions();
}

void GameFragment::onCheckPressed() {
    QSound::play(":/music/click");
    SetCheck(0);
    Client->GameCheck();
    BlockActions();
}

void GameFragment::onRaisePressed() {
    QSound::play(":/music/click");
    auto bet = BetValue->text().toInt();
    if (bet > mMaxbet) {
        bet = mMaxbet;
        BetValue->setText(QString::number(bet));
    }
    if (bet < mMinbet) {
        bet = mMinbet;
        BetValue->setText(QString::number(bet));
    }
    SetRaise(0);
    mChips->AddToBank(bet);
    Client->GameRaise(BetValue->text().toUInt());
    BlockActions();
}

void GameFragment::onFoldPressed() {
    QSound::play(":/music/click");
    SetFold(0);
    Client->GameFold();
    BlockActions();
}

void GameFragment::onLeavePressed() {
    QSound::play(":/music/click");
    Client->LeaveRoom();
    back();
}

void GameFragment::onSettingsPressed() {
    QSound::play(":/music/click");
    navigateTo(SETTINGS_TAG);
}

void GameFragment::onStartPressed() {
    QSound::play(":/music/click");
    Client->StartGame();
}

void GameFragment::HideStart() {
    StartGameButton->hide();
}

void GameFragment::EndGame(bool admin) {
    foreach (auto btn, ActionButtons) {
        btn->hide();
    }
    BetSlider->hide();
    BetValue->hide();
    if (admin)
    StartGameButton->show();
}

void GameFragment::AvaliableActions(std::vector<bool> buttons) {
    for(size_t i = 0; i < buttons.size(); ++i) {
        buttons[i] ? ActionButtons[i]->show() : ActionButtons[i]->hide();
    }
}

void GameFragment::JoinNotAdmin() {
    StartGameButton->hide();
}

void GameFragment::SetMinBet(int minbet) {
    if (minbet > 0) {
        mMinbet = minbet;
        BetSlider->setRange(mMinbet, mMaxbet);
        QString value = QString::number(mMinbet);
        BetValue->setText(value);
        BetValue->setValidator(new QIntValidator(mMinbet, mMaxbet, this));
    }
}

void GameFragment::SetMaxBet(int maxbet) {
    if (maxbet > 0) {
        mMaxbet = maxbet;
        BetSlider->setRange(mMinbet, mMaxbet);
        BetValue->setValidator(new QIntValidator(mMinbet, mMaxbet, this));
    }
}

void GameFragment::DrawPlayer(size_t player_id, std::string nickname, size_t total_money) {
    OtherPlayer* player = new OtherPlayer(player_id, nickname, total_money);
    mOtherPlayers[player_id - 1] = player;
    mOtherPlayers[player_id - 1]->setParent(mPlayTable);
    switch (player_id) {
        case 1: {
            mOtherPlayers[player_id - 1]->setGeometry(firstpos);
            mOtherPlayers[player_id - 1]->SetPosition(firstpos);
        break;
        }
        case 2: {
            mOtherPlayers[player_id - 1]->setGeometry(secondpos);
            mOtherPlayers[player_id - 1]->SetPosition(secondpos);
        break;
        }
        case 3: {
            mOtherPlayers[player_id - 1]->setGeometry(thirdpos);
            mOtherPlayers[player_id - 1]->SetPosition(thirdpos);
        break;
        }
        case 4: {
            mOtherPlayers[player_id - 1]->setGeometry(fouthpos);
            mOtherPlayers[player_id - 1]->SetPosition(fouthpos);
        break;
        }
        case 5: {
            mOtherPlayers[player_id - 1]->setGeometry(fifthpos);
            mOtherPlayers[player_id - 1]->SetPosition(fifthpos);
        break;
        }
    }
}

void GameFragment::DeletePlayer(size_t player_id) {
    delete mOtherPlayers[player_id - 1];
    mOtherPlayers[player_id - 1] = nullptr;
}

void GameFragment::DrawMainPlayer() {
    mPlayer = new Player("ME :)", 9999);
//    mPlayer = new Player(LoggedUserName, LoggedUserMoney);
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

void GameFragment::SetCall(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->setCall();
    } else {
        mPlayer->setCall();
    }
}

void GameFragment::SetFold(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->setFold();
        mOtherPlayers[player_id - 1]->DiscardCards();
    } else {
        mPlayer->setFold();
        mPlayer->DiscardCards();
    }
}

void GameFragment::SetCheck(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->setCheck();
    } else {
        mPlayer->setCheck();
    }
}

void GameFragment::SetRaise(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->setRaise();
    } else {
        mPlayer->setRaise();
    }
}

void GameFragment::SetBet(size_t player_id, size_t bet) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->setBet(bet);
    } else {
        mPlayer->setBet(bet);
    }
}

void GameFragment::ClearStatus(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->ClearStatus();
    } else {
        mPlayer->ClearStatus();
    }
}

void GameFragment::FlipCards(size_t player_id) {
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->FlipCards();
    } else {
        mPlayer->FlipCards();
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

void GameFragment::DisplayWinner(size_t player_id) {
    QString text;
    if (player_id > 0) {
        mOtherPlayers[player_id - 1]->AddMoney(mChips->GetBank());
        text = "Winner is " + mOtherPlayers[player_id - 1]->GetName() + ". Won " + QString::number(mChips->GetBank()) + "$";
    } else {
        mPlayer->AddMoney(mChips->GetBank());
        text = "Winner is " + mPlayer->GetName() + ". Won " + QString::number(mChips->GetBank()) + "$";
    }

    mChips->Wipe();
    mWinLabel->setText(text);
    mWinLabel->setAlignment(Qt::AlignCenter);

    mWinLabel->show();
}

void GameFragment::CurrentTurn(size_t player_id) {
    if (player_id > 0) {
        mTurnIndicator->setParent(mOtherPlayers[player_id - 1]);
        mTurnIndicator->setGeometry(90, -80, 300, 300);
    } else {
        mTurnIndicator->setParent(mPlayer);
        mTurnIndicator->setGeometry(85, -25, 300, 300);
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

void GameFragment::FlipTableCards() {
    foreach(auto card, CardOnTable) {
        card->Flip();
    }
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
        if (player != nullptr) {
            player->Resize(this->size());
            if(player->HasCards) {
                player->GetCard().first->Resize(this->size());
                player->GetCard().second->Resize(this->size());
            }
            RedrawPlayer(player);
        }
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

void GameFragment::ShowActions() {
    if (is_active) {
        return;
    }

    foreach (auto btn, ActionButtons) {
        btn->show();
    }
    BetSlider->show();
    BetValue->show();
    StartGameButton->hide();
}

