#include "playtable.h"

PlayTable::PlayTable() {
    mypix = new QImage("/home/zennoma/Full-House/App/Media/table.png");
    table = mypix->scaled(960, 720, Qt::IgnoreAspectRatio);

    this->setPixmap(QPixmap::fromImage(table, Qt::AutoColor));
    this->resize(640, 480);
}


PlayTable::~PlayTable() {
    delete mypix;
}
