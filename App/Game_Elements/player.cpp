#include "player.h"

Player::Player(std::string name, size_t money) {
    mPlayerInfo = new PlayerInfoLabel(name, money);

    mypix = new QImage(":/players/Kyle");

    player = mypix->scaled(200, 200, Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
    this->resize(200, 200);
    this->setStyleSheet("margin:50px;margin-left:80px");

    mPlayerInfo->setParent(this);
    mPlayerInfo->setGeometry(QRect(150, 50, 400, 300));

}

Player::~Player() {
    delete mChips;
    delete mPlayerInfo;
    delete mTexture;
}

void Player::DisplayCards() {

}
