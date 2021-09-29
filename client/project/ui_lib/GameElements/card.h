#pragma once
#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>


class Card : public QLabel {
public:
    Card() = delete;
    explicit Card(size_t const& value, size_t const& suit, bool const& upsided = false);
    ~Card();
    void Flip();
    size_t GetValue();
    size_t GetSuit();
    bool GetSide();

    void Resize(QSize WinSize);

    void ResizeOnTable(QSize WinSize, size_t pos);

private:
    bool LowRes;
    QSize normal;
    QSize min;
    bool mUpSided;
    size_t mUpSideTextureId;
    size_t mDownSideTextureId = 0;
    size_t mValue;
    size_t mSuit;
    QImage* mUpTexture;
    QImage* mDownTexture;

    QImage card;
};

#endif // CARD_H
