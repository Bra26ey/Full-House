#include "playtable.h"

PlayTable::PlayTable() {
    mypix = new QImage(":/other_textures/Media/table.png");
    normal.setWidth(960);
    normal.setHeight(720);
    min.setWidth(800);
    min.setHeight(600);
    table = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
    this->setStyleSheet("margin-left:200px;margin-top:200px;margin-right:200px");
}


PlayTable::~PlayTable() {
    delete mypix;
}

void PlayTable::Resize(QSize WinSize) {
    if (WinSize.height() <= 1093) {
        table = mypix->scaled(min.width(), min.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
        this->setStyleSheet("margin-left:166px;margin-top:166px;margin-right:166px");
    } else {
        table = mypix->scaled(normal.width(), normal.height(), Qt::IgnoreAspectRatio);
        this->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
        this->setStyleSheet("margin-left:200px;margin-top:200px;margin-right:200px");
    }
}
