#include "turnspark.h"

TurnSpark::TurnSpark() {
    mypix = new QImage(":/other_textures/spark");
    spark = mypix->scaled(35, 35, Qt::KeepAspectRatio);

    this->setPixmap(QPixmap::fromImage(spark, Qt::AutoColor));
}

TurnSpark::~TurnSpark() {
    delete mypix;
}
