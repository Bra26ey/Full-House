#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QPixmap>


class Card: public QWidget {
public:
    Card() = delete;
    explicit Card(size_t up_texture, size_t down_texture, size_t value, size_t suit);
    ~Card();
    size_t GetUpTextureId();
    size_t GetDownTextureId();
    size_t GetValue();
    size_t GetSuit();


private:
    void setTextures();

    size_t mUpSideTextureId;
    size_t mDownSideTextureId;
    size_t mValue;
    size_t mSuit;
    QPixmap* mUpTexture;
    QPixmap* mDownTexture;
};

#endif // CARD_H
