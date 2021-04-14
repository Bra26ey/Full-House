#ifndef PLAYER_H
#define PLAYER_H

#include "otherplayer.h"

class Player: public OtherPlayer {
public:
    Player();
    ~Player();
    void DisplayCards();
};

#endif // PLAYER_H
