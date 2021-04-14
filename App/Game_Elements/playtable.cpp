#include "playtable.h"

PlayTable::PlayTable(size_t texture_id): mTextureId(texture_id) {

}


PlayTable::~PlayTable() {
    delete mTexture;
}
