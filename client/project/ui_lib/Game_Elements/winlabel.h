#ifndef WINLABEL_H
#define WINLABEL_H

#include <QLabel>

class WinLabel : public QLabel {
public:
    WinLabel();
    void Resize(QSize WinSize);
};

#endif // WINLABEL_H
