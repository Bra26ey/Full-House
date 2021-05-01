#include "player.h"

Player::Player(size_t pos) : OtherPlayer(pos) {

}

Player::~Player() {
    delete Chips;
    delete[] Cards;
    delete PlayerInfo;
    delete mTexture;
}

void Player::DisplayCards() {

}
