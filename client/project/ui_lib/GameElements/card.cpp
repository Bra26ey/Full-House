#include "card.h"

#include <QString>
#include <QDebug>


Card::Card(size_t const& value, size_t const&  suit, bool const& upsided): mUpSided(upsided), mValue(value), mSuit(suit) {
    mUpSideTextureId = (mSuit * 13) + value;

    normal.setWidth(70);
    normal.setHeight(90);
    min.setWidth(63);
    min.setHeight(81);

    QString name = "k" + QString::number(mUpSideTextureId) + ".png";
    qDebug() << name;
    mUpTexture = new QImage(":/cards/Media/Cards/" + name);
    name = "k0.png";
    mDownTexture = new QImage(":/cards/Media/Cards/" + name);

    if (upsided) {
        card = mUpTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
    } else {
        card = mDownTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
    }
    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
    this->repaint();
}


Card::~Card() {
    delete mDownTexture;
    delete mUpTexture;
}

void Card::Flip() {
    if (!LowRes)
    if (mUpSided) {
        card = mDownTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        mUpSided = false;
    } else {
        card = mUpTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        mUpSided = true;
    } else {
        if (mUpSided) {
            card = mDownTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
            mUpSided = false;
        } else {
            card = mUpTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
            mUpSided = true;
        }
    }
    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
    this->repaint();
}

size_t Card::GetValue() {
    return mValue;
}

size_t Card::GetSuit() {
    return mSuit;
}

bool Card::GetSide() {
    return mUpSided;
}

void Card::Resize(QSize WinSize) {
    if (WinSize.height() <= 1093) {
        if (mUpSided)
        card = mUpTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        else card = mDownTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        LowRes = 1;
    } else {
        if (mUpSided)
        card = mUpTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        else card = mDownTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        LowRes = 0;
    }
    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
}


void Card::ResizeOnTable(QSize WinSize, size_t pos) {
    if (WinSize.height() <= 1093) {
        if (mUpSided)
        card = mUpTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        else card = mDownTexture->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        LowRes = 1;
        this->setGeometry(166 + pos * 80, 80, 500, 500);
    } else {
        if (mUpSided)
        card = mUpTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        else card = mDownTexture->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        LowRes = 0;
        this->setGeometry(280 + pos * 90, 150, 500, 500);
    }
    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
}
