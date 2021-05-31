#include "gamefragment.h"
#include "mainwindow.h"

#include <QDebug>
#include <QSound>
#include <QVector>
#include <QIntValidator>
#include <QThread>
#include <QMessageBox>


int base_card_coefficient = 280;
int card_move_coefficient = 90;

int min_base_card_coefficient = 166;
int min_card_move_coefficient = 80;

using namespace screens;
GameFragment::GameFragment() : mMinbet(1), mMaxbet(10), mOtherPlayers(6, nullptr) {
    qDebug() << QThread::currentThreadId();
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
    StartGameButton->hide();

    LeaveButton = new QPushButton("Exit");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &GameFragment::onLeavePressed);

    ShowRoomInfo = new QPushButton("Show Info");
    ShowRoomInfo->setStyleSheet("color:#242424;font-size:24px");
    connect(ShowRoomInfo, &QPushButton::clicked, this, &GameFragment::onInfoPressed);

    extraHLayout->addWidget(SettingsButton);
    extraHLayout->addWidget(StartGameButton);
    extraHLayout->addWidget(ShowRoomInfo);
    extraHLayout->addWidget(LeaveButton);

    mainVLayout->addLayout(extraHLayout);
    this->setLayout(mainVLayout);

    is_active = false;
}

GameFragment::~GameFragment() {
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
    delete ShowRoomInfo;
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
    SetBet(0, bet);
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
    SetRaise(0, bet);
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
}


void GameFragment::onInfoPressed() {
    QSound::play(":/music/click");
    QMessageBox msgBox;
    msgBox.setText("RoomId is " + QString::number(globalInfo::RoomId)+"\nRoomPassword is " + globalInfo::Password.c_str());
    msgBox.setWindowTitle("RoomInfo");
    msgBox.exec();
}

void GameFragment::onSettingsPressed() {
    QSound::play(":/music/click");
    navigateTo(SETTINGS_TAG);
}

void GameFragment::onStartPressed() {
    QSound::play(":/music/click");
    Client->StartGame();
}

void GameFragment::ShowStart() {
    StartGameButton->show();
    this->update();
    this->show();
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
    mOtherPlayers[player_id] = player;
    mOtherPlayers[player_id]->setParent(mPlayTable);
    switch (player_id) {
        case 0: {
            mOtherPlayers[player_id]->setGeometry(mainplayerpos);
            mOtherPlayers[player_id]->SetPosition(mainplayerpos);
        break;
        }
        case 1: {
            mOtherPlayers[player_id]->setGeometry(firstpos);
            mOtherPlayers[player_id]->SetPosition(firstpos);
        break;
        }
        case 2: {
            mOtherPlayers[player_id]->setGeometry(secondpos);
            mOtherPlayers[player_id]->SetPosition(secondpos);
        break;
        }
        case 3: {
            mOtherPlayers[player_id]->setGeometry(thirdpos);
            mOtherPlayers[player_id]->SetPosition(thirdpos);
        break;
        }
        case 4: {
            mOtherPlayers[player_id]->setGeometry(fouthpos);
            mOtherPlayers[player_id]->SetPosition(fouthpos);
        break;
        }
        case 5: {
            mOtherPlayers[player_id]->setGeometry(fifthpos);
            mOtherPlayers[player_id]->SetPosition(fifthpos);
        break;
        }
    }
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::DeletePlayer(size_t player_id) {
    if ((player_id > 0 && player_id < 6) || mOtherPlayers[player_id] != nullptr) {
        delete mOtherPlayers[player_id];
        mOtherPlayers[player_id] = nullptr;
    }
}

void GameFragment::GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2) {
    mOtherPlayers[player_id]->GiveCards(value1, suit1, value2, suit2);
    if (player_id == 0) {
        mOtherPlayers[player_id]->FlipCards();
    }
    mOtherPlayers[player_id]->GetCard().first->show();
    mOtherPlayers[player_id]->GetCard().second->show();
}

void GameFragment::SetCall(size_t player_id) {
    mOtherPlayers[player_id]->setCall();
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::SetFold(size_t player_id) {
    mOtherPlayers[player_id]->setFold();
    mOtherPlayers[player_id]->DiscardCards();
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::SetCheck(size_t player_id) {
    mOtherPlayers[player_id]->setCheck();
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::SetRaise(size_t player_id, size_t bet) {
    mOtherPlayers[player_id]->setRaise(bet);
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::SetBet(size_t player_id, size_t bet) {
    mOtherPlayers[player_id]->setBet(bet);
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::SetMoney(size_t player_id, size_t money) {
    mOtherPlayers[player_id]->setMoney(money);
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::ClearStatus(size_t player_id) {
        mOtherPlayers[player_id]->ClearStatus();
        this->resizeEvent(nullptr);
        mOtherPlayers[player_id]->show();
}

void GameFragment::FlipCards(size_t player_id) {
    if (mOtherPlayers[player_id]->GetHasCards()) {
        mOtherPlayers[player_id]->FlipCards();
        this->resizeEvent(nullptr);
        mOtherPlayers[player_id]->show();
    }
}


void GameFragment::MakeDealer(size_t player_id) {
    mDealerLogo->setParent(mOtherPlayers[player_id]);
    if (player_id > 0) {
        mDealerLogo->setGeometry(10, 20, 200, 200);
    } else {
        mDealerLogo->setGeometry(-20, -10, 200, 200);
    }
    this->resizeEvent(nullptr);
    mOtherPlayers[player_id]->show();
}

void GameFragment::FlipAllCards() {
    foreach(auto player, mOtherPlayers) {
        if (player != nullptr) {
            if (!player->GetCardSide())
            player->FlipCards();
        }
    }
    this->resizeEvent(nullptr);
    mPlayTable->show();
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

//    mOtherPlayers[player_id]->AddMoney(mChips->GetBank());
    text = "Winner is " + mOtherPlayers[player_id]->GetName() + ". Won " + QString::number(mChips->GetBank()) + "$";

    mWinLabel->setText(text);
    mWinLabel->setAlignment(Qt::AlignCenter);

    mWinLabel->show();
}

void GameFragment::CurrentTurn(size_t player_id) {
    mTurnIndicator->setParent(mOtherPlayers[player_id]);
    if (player_id > 0) {
        mTurnIndicator->setGeometry(90, -80, 300, 300);
    } else {
        mTurnIndicator->setGeometry(85, -25, 300, 300);
    }
    this->resizeEvent(nullptr);
    mTurnIndicator->show();
}

void GameFragment::DeleteWinnerDisplay() {
    mWinLabel->hide();
}

void GameFragment::AddCardToTable(size_t value,  size_t suit,  bool upsided) {
    auto card = new Card(value, suit, upsided);
    card->setParent(mPlayTable);
    CardOnTable.push_back(card);
    this->resizeEvent(nullptr);
    card->show();
}

void GameFragment::SetMoneyInBank(size_t money) {
    mChips->Wipe();
    mChips->AddToBank(money);
    mChips->show();
}

void GameFragment::ClearBank() {
    mChips->Wipe();
}

void GameFragment::FlipTableCards() {
    foreach(auto card, CardOnTable) {
        card->Flip();
    }
    this->resizeEvent(nullptr);
    mPlayTable->show();
}

void GameFragment::DeleteAllCardsFromTable() {
    foreach(auto card, CardOnTable) {
        if (card != nullptr)
        card->deleteLater();
    }
    while(!CardOnTable.empty())
        CardOnTable.pop_front();
    this->resizeEvent(nullptr);
    mPlayTable->show();
}

void GameFragment::DeleteAllPlayersCards() {
    foreach(auto player, mOtherPlayers) {
        if (player != nullptr)
        if (player->GetHasCards()) {
            player->DiscardCards();
        }
    }
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
            if(player->GetHasCards()) {
                player->GetCard().first->Resize(this->size());
                player->GetCard().second->Resize(this->size());
            }
            RedrawPlayer(player);
        }
    }
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

