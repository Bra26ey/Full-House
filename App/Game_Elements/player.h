#ifndef PLAYER_H
#define PLAYER_H

#include "otherplayer.h"

class Player: public OtherPlayer {
public:
    Player();
    ~Player();
    void DisplayCards();

private:
    size_t mTextureId;
    QPixmap *mTexture;
};

#endif // PLAYER_H
