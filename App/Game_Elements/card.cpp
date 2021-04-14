#include "card.h"

Card::Card(size_t up_texture, size_t down_texture, size_t value, size_t suit): mUpSideTextureId(up_texture), mDownSideTextureId(down_texture),
mValue(value), mSuit(suit) {

}


Card::~Card() {
    delete mDownTexture;
    delete mUpTexture;
}

size_t Card::GetDownTextureId() {
    return mDownSideTextureId;
}

size_t Card::GetUpTextureId() {
    return mUpSideTextureId;
}

size_t Card::GetValue() {
    return mValue;
}

size_t Card::GetSuit() {
    return mSuit;
}

void Card::setTextures() {
}
