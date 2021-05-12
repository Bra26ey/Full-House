#include "player.h"

Player::Player(std::string name, size_t money) {
    mPlayerInfo = new PlayerInfoLabel(name, money);

    mypix = new QImage(":/players/Kyle");

    normal.setHeight(200);
    normal.setWidth(200);
    min.setHeight(166);
    min.setWidth(166);

    player = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
    this->setStyleSheet("margin:50px;margin-left:80px");

    mPlayerInfo->setParent(this);
    mPlayerInfo->setGeometry(QRect(100, 60, 400, 300));

}

Player::~Player() {
    delete mChips;
    delete mPlayerInfo;
    delete mTexture;
}

void Player::Resize(QSize WinSize) {
    if (WinSize.width() <= 1378 && WinSize.height() <= 1093) {
        player = mypix->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
        LowRes = 1;
        posChange();
    } else {
        player = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
        LowRes = 0;
        posChange();
    }
}

void Player::posChange() {
    if (LowRes) {
        qDebug("MinSet");
        if (mPos == mainplayerpos) {
            mPos = mainplayerposmin;
        }
    } else {
        if (mPos == mainplayerposmin) {
            mPos = mainplayerpos;
        }
    }
}


