#ifndef PLAYTABLE_H
#define PLAYTABLE_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QImage>

class PlayTable:public QLabel {
public:
    PlayTable();
    ~PlayTable();

private:
    QImage *mypix;
    QImage table;
};

#endif // PLAYTABLE_H
