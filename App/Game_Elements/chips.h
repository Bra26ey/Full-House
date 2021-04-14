#ifndef CHIPS_H
#define CHIPS_H

#include <QWidget>
#include <QPixmap>

class Chips: public QWidget {
public:
    Chips() = delete;
    Chips(size_t value);
    ~Chips();

private:
    void setTextures();
    QPixmap* mTexture;
    size_t mValue;
};

#endif // CHIPS_H
