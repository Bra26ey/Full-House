#ifndef CHIPS_H
#define CHIPS_H

#include <QLabel>
#include <QPixmap>
#include <QImage>

class Chips: public QLabel {
public:
    Chips();
    ~Chips();
    void Wipe();
    void AddToBank(size_t value);
    size_t GetBank();

private:
    QImage* mypix;
    QImage chips;
    size_t mValue;
    QLabel* mAmount;
};

#endif // CHIPS_H
