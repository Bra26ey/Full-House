#include "otherplayer.h"
#include <QDebug>

OtherPlayer::OtherPlayer(size_t id, std::string nickname, size_t money) :  HasCards(false), LowRes(0) {
    mPlayerInfo = new PlayerInfoLabel(nickname, money);
    switch(id) {
    case 0: {
        mypix = new QImage(":/players/Kyle");
        break;
    }
    case 1: {
        mypix = new QImage(":/players/Cartman");
        break;
    }
    case 2: {
        mypix = new QImage(":/players/Kenny");
        break;
    }
    case 3: {
        mypix = new QImage(":/players/Stan");
        break;
    }
    case 4: {
        mypix = new QImage(":/players/Wendy");
        break;
    }
    case 5: {
        mypix = new QImage(":/players/Dougie");
        break;
    }
    }

    normal.setWidth(200);
    normal.setHeight(200);
    min.setWidth(166);
    min.setHeight(166);
    player = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
    this->setStyleSheet("margin:50px;margin-left:100px");

    mPlayerInfo->setParent(this);

    if (id == 0) {
        mPlayerInfo->setGeometry(QRect(100, 50, 400, 300));
    } else {
        mPlayerInfo->setGeometry(QRect(100, 0, 400, 300));
    }
}

OtherPlayer::~OtherPlayer() {
    delete mypix;
}


void OtherPlayer::setBet(size_t bet) {
    mPlayerInfo->setBet(bet);
}

void OtherPlayer::setCheck() {
    mPlayerInfo->setCheck();
}

void OtherPlayer::setFold() {
    mPlayerInfo->setFold();
}

void OtherPlayer::setRaise(size_t bet) {
    mPlayerInfo->setRaise(bet);
}

void OtherPlayer::ClearStatus() {
    mPlayerInfo->ClearStatus();
}

void OtherPlayer::setCall() {
    mPlayerInfo->setCall();
}

void OtherPlayer::setMoney(size_t money) {
    mPlayerInfo->setMoney(money);
}

void OtherPlayer::SetPosition(QRect &pos) {
    mPos = pos;
}

void OtherPlayer::GiveCards(size_t value1, size_t suit1, size_t value2, size_t suit2) {
    Card* firstCard = new Card(value1, suit1);
    Card* secondCard = new Card(value2, suit2);
    HasCards = true;

    mCards.first = firstCard;
    mCards.second = secondCard;

    mCards.first->setParent(this);
    mCards.second->setParent(this);
    setCardPos();
}

void OtherPlayer::DiscardCards() {
    if (HasCards == true) {
        HasCards = false;
        mCards.first->deleteLater();
        mCards.second->deleteLater();
    }
}

void OtherPlayer::FlipCards() {
    if (HasCards) {
        mCards.first->Flip();
        mCards.second->Flip();
    }
}

bool OtherPlayer::GetCardSide() {
    if (HasCards) {
        return mCards.first->GetSide();
    }
    return false;
}

bool OtherPlayer::GetHasCards() {
    return HasCards;
}

size_t OtherPlayer::GetBet() {
    return mPlayerInfo->GetBet();
}

void OtherPlayer::AddMoney(size_t add) {
    size_t temp = mPlayerInfo->GetTotal();
    mPlayerInfo->setTotal(temp + add);
}

QString OtherPlayer::GetName() {
    return mPlayerInfo->GetName();
}

QRect OtherPlayer::GetPos() {
    return mPos;
}

QPair<Card*, Card*> OtherPlayer::GetCard() {
    return mCards;
}

void OtherPlayer::Resize(QSize WinSize) {
    if (WinSize.height() <= 1093) {
        player = mypix->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
        this->setStyleSheet("margin:50px;margin-left:80px");
        LowRes = 1;
    } else {
        player = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
        this->setStyleSheet("margin:50px;margin-left:100px");
        LowRes = 0;
    }
    posChange();
    setCardPos();
}

void OtherPlayer::setCardPos() {
    if(HasCards) {
        if(!LowRes) {
            if (mPos == mainplayerpos) {
                mCards.first->setGeometry(40, -50, 220, 220);
                mCards.second->setGeometry(90, -50, 220, 220);
            } else if (mPos == firstpos) {
                mCards.first->setGeometry(180, 80, 220, 220);
                mCards.second->setGeometry(220, 80, 220, 220);
            } else if (mPos == secondpos) {
                mCards.first->setGeometry(150, 220, 220, 220);
                mCards.second->setGeometry(210, 220, 220, 220);
            } else if (mPos == thirdpos) {
                mCards.first->setGeometry(30, 245, 220, 220);
                mCards.second->setGeometry(90, 245, 220, 220);
            } else if (mPos == fouthpos) {
                mCards.first->setGeometry(-50, 220, 220, 220);
                mCards.second->setGeometry(10, 220, 220, 220);
            } else if (mPos == fifthpos) {
                mCards.first->setGeometry(-100, 80, 220, 220);
                mCards.second->setGeometry(-60, 80, 220, 220);
            }
        } else {
            if (mPos == mainplayerposmin) {
                mCards.first->setGeometry(35, -45, 220, 220);
                mCards.second->setGeometry(85, -45, 220, 220);
            } else if (mPos == firstposmin) {
                mCards.first->setGeometry(160, 80, 220, 220);
                mCards.second->setGeometry(200, 80, 220, 220);
            } else if (mPos == secondposmin) {
                mCards.first->setGeometry(130, 210, 220, 220);
                mCards.second->setGeometry(190, 210, 220, 220);
            } else if (mPos == thirdposmin) {
                mCards.first->setGeometry(30, 235, 220, 220);
                mCards.second->setGeometry(90, 235, 220, 220);
            } else if (mPos == fouthposmin) {
                mCards.first->setGeometry(-50, 210, 220, 220);
                mCards.second->setGeometry(10, 210, 220, 220);
            } else if (mPos == fifthposmin) {
                mCards.first->setGeometry(-80, 40, 220, 220);
                mCards.second->setGeometry(-40, 40, 220, 220);
            }
        }
    }
}

void OtherPlayer::posChange() {
    if (LowRes) {
        if (mPos == mainplayerpos) {
            mPos = mainplayerposmin;
        } else if (mPos == firstpos) {
            mPos = firstposmin;
        } else if (mPos == secondpos) {
            mPos = secondposmin;
        } else if (mPos == thirdpos) {
            mPos = thirdposmin;
        } else if (mPos == fouthpos) {
            mPos = fouthposmin;
        } else if (mPos == fifthpos) {
            mPos = fifthposmin;
        }
    } else {
        if (mPos == mainplayerposmin) {
            mPos = mainplayerpos;
        } else if (mPos == firstposmin) {
            mPos = firstpos;
        } else if (mPos == secondposmin) {
            mPos = secondpos;
        } else if (mPos == thirdposmin) {
            mPos = thirdpos;
        } else if (mPos == fouthposmin) {
            mPos = fouthpos;
        } else if (mPos == fifthposmin) {
            mPos = fifthpos;
        }
    }
}
