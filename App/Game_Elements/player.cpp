#include "player.h"

Player::Player() {

}

Player::~Player() {
    delete Chips;
    delete[] Cards;
    delete PlayerInfo;
}

void Player::DisplayCards() {

}
