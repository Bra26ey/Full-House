#include "otherplayer.h"

OtherPlayer::OtherPlayer(size_t id) {
    switch(id) {
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

    player = mypix->scaled(200, 200, Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(player, Qt::AutoColor));
    this->resize(200, 200);

}

OtherPlayer::~OtherPlayer() {
    delete Chips;
    delete[] Cards;
    delete PlayerInfo;
    delete mypix;
}

void OtherPlayer::DisplayInfo() {

}

void OtherPlayer::DisplayPlayer() {

}

void OtherPlayer::SetPosition(QRect &pos) {
    mPos = pos;
}
