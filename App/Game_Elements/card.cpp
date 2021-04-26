#include "card.h"

#include <QString>
#include <QDebug>


Card::Card(size_t value, size_t suit): mValue(value), mSuit(suit) {
    mUpSideTextureId = mValue * mSuit;


    QString name = "k" + QString::number(mUpSideTextureId) + ".png";
    qDebug() << name;
    mUpTexture = new QImage("/home/zennoma/Full-House/App/Media/Cards/" + name);
    card = mUpTexture->scaled(70, 90, Qt::IgnoreAspectRatio);


    this->setPixmap(QPixmap::fromImage(card, Qt::AutoColor));
    this->repaint();
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

