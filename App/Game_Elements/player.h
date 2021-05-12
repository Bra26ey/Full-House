#ifndef PLAYER_H
#define PLAYER_H

#include "otherplayer.h"

class Player: public OtherPlayer {
public:
    Player(std::string name, size_t money);
    ~Player();
    void Resize(QSize WinSize) override;

private:
    void posChange();
    bool LowRes;
    QSize normal;
    QSize min;
    size_t mTextureId;
    QPixmap *mTexture;
};

#endif // PLAYER_H
