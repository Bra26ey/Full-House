#include "winlabel.h"

WinLabel::WinLabel() {
    this->setStyleSheet("background:rgba(33,33,33,0.5);font-size:26px;color:rgb(0, 255, 255);");
    this->hide();
}


void WinLabel::Resize(QSize WinSize) {
    if (WinSize.height() <= 1093) {
        this->setGeometry(205, 290, 780, 200);
    } else {
        this->setGeometry(320, 350, 780, 250);
    }
}
