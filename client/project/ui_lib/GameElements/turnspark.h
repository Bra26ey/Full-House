#ifndef TURNSPARK_H
#define TURNSPARK_H

#include <QPixmap>
#include <QLabel>
#include <QImage>

class TurnSpark : public QLabel {
public:
    TurnSpark();
    ~TurnSpark();
private:
    QImage *mypix;
    QImage spark;
};

#endif // TURNSPARK_H
