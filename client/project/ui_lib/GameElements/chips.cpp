#include "chips.h"

Chips::Chips() : mValue(0) {
    mypix = new QImage(":/other_textures/chips");
    normal.setWidth(120);
    normal.setHeight(120);

    min.setWidth(100);
    min.setHeight(100);

    chips = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(chips, Qt::AutoColor));

    mAmount = new QLabel;
    QString text = "Total $: " + QString::number(mValue);
    mAmount->setParent(this);
    mAmount->setText(text);
    mAmount->setStyleSheet("background:rgba(33,33,33,0.7);font-size:20px;color:rgb(5, 189, 240);");
    mAmount->setGeometry(-25, 165, 550, 225);
    mAmount->show();

}

void Chips::Wipe() {
    mValue = 0;
    QString text = "Total $: " + QString::number(mValue);
    mAmount->setText(text);
}

void Chips::AddToBank(size_t value) {
    mValue += value;
    QString text = "Total $: " + QString::number(mValue);
    mAmount->setText(text);
}

Chips::~Chips() {
    delete mypix;
    delete mAmount;
}

size_t Chips::GetBank() {
    return mValue;
}

void Chips::Resize(QSize WinSize) {
    if (WinSize.height() <= 1093) {
        chips = mypix->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(chips, Qt::AutoColor));
        LowRes = 1;
    } else {
        chips = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(chips, Qt::AutoColor));
        LowRes = 0;
    }
    ChangePos();
}

void Chips::ChangePos() {
    if(LowRes) {
        mAmount->setGeometry(-25, 165, 500, 200);
        Pos = QRect(580, 90, 480, 500);
    } else {
        mAmount->setGeometry(-25, 165, 550, 225);
        Pos = QRect(760, 140, 780, 550);
    }
    this->setGeometry(Pos);
}
