#pragma once
#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

enum cards {
    diamond_2 = 1,
    diamond_3 = 2,
    diamond_4 = 3,
    diamond_5 = 4,
    diamond_6 = 5,
    diamond_7 = 6,
    diamond_8 = 7,
    diamond_9 = 8,
    diamond_10 = 9,
    diamond_jack = 10,
    diamond_queen = 11,
    diamond_king = 12,
    diamond_ace = 13,

    heart_2 = 14,
    heart_3 = 15,
    heart_4 = 16,
    heart_5 = 17,
    heart_6 = 18,
    heart_7 = 19,
    heart_8 = 20,
    heart_9 = 21,
    heart_10 = 22,
    heart_jack = 23,
    heart_queen = 24,
    heart_king = 25,
    heart_ace = 26,

    club_2 = 27,
    club_3 = 28,
    club_4 = 29,
    club_5 = 30,
    club_6 = 31,
    club_7 = 32,
    club_8 = 33,
    club_9 = 34,
    club_10 = 35,
    club_jack = 36,
    club_queen = 37,
    club_king = 38,
    club_ace = 39,

    spade_2 = 40,
    spade_3 = 41,
    spade_4 = 42,
    spade_5 = 43,
    spade_6 = 44,
    spade_7 = 45,
    spade_8 = 46,
    spade_9 = 47,
    spade_10 = 48,
    spade_jack = 49,
    spade_queen = 50,
    spade_king = 51,
    spade_ace = 52,

    blank = 53
};


class Card : public QLabel {
public:
    Card() = delete;
    explicit Card(size_t value, size_t suit);
    ~Card();
    size_t GetUpTextureId();
    size_t GetDownTextureId();
    size_t GetValue();
    size_t GetSuit();


private:
    size_t mUpSideTextureId;
    size_t mDownSideTextureId = cards::blank;
    size_t mValue;
    size_t mSuit;
    QImage* mUpTexture;
    QImage* mDownTexture;

    QImage card;
};

#endif // CARD_H
