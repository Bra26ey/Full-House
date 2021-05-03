#include "card.h"

#include <QString>
#include <QDebug>


Card::Card(size_t value, size_t suit, bool upsided): mUpSided(upsided), mValue(value), mSuit(suit) {
    mUpSideTextureId = ((mSuit - 1) * 13) + value;


    QString name = "k" + QString::number(mUpSideTextureId) + ".png";
    qDebug() << name;
    mUpTexture = new QImage(":/cards/Media/Cards/" + name);
    name = "k0.png";
    mDownTexture = new QImage(":/cards/Media/Cards/" + name);

    if (upsided) {
        card = mUpTexture->scaled(70, 90, Qt::IgnoreAspectRatio);
    } else {
        card = mDownTexture->scaled(70, 90, Qt::IgnoreAspectRatio);
    }
    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
    this->repaint();
}


Card::~Card() {
    delete mDownTexture;
    delete mUpTexture;
}

void Card::Flip() {
    if (mUpSided) {
        card = mDownTexture->scaled(70, 90, Qt::IgnoreAspectRatio);
        mUpSided = false;
    } else {
        card = mUpTexture->scaled(70, 90, Qt::IgnoreAspectRatio);
        mUpSided = true;
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
