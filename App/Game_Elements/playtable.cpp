#include "playtable.h"

PlayTable::PlayTable() {
    mypix = new QImage(":/other_textures/Media/table.png");
    table = mypix->scaled(960, 720, Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
    this->resize(640, 480);
    this->setStyleSheet("margin-left:200px;margin-top:200px;margin-right:200px");
}


PlayTable::~PlayTable() {
    delete mypix;
}
