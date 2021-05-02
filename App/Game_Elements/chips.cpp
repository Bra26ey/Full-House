#include "chips.h"

Chips::Chips() : mValue(0) {
    mypix = new QImage(":/other_textures/chips");
    chips = mypix->scaled(120, 120, Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(chips, Qt::AutoColor));
    this->resize(120, 120);

    mAmount = new QLabel;
    QString text = "Total $: " + QString::number(mValue);
    mAmount->setParent(this);
    mAmount->setText(text);
    mAmount->setStyleSheet("background:rgba(33,33,33,0.7);font-size:20px;color:rgb(2, 116, 245);");
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
