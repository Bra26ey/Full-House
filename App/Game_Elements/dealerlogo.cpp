#include "dealerlogo.h"

DealerLogo::DealerLogo() {
    mypix = new QImage(":/other_textures/DealerLogo");
    logo = mypix->scaled(40, 40, Qt::IgnoreAspectRatio);


    this->setPixmap(QPixmap::fromImage(logo, Qt::AutoColor));
    this->resize(40, 40);
}

DealerLogo::~DealerLogo() {
    delete mypix;
}
