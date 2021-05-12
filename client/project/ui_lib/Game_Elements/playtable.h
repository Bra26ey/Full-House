#ifndef PLAYTABLE_H
#define PLAYTABLE_H

#include <QPixmap>
#include <QLabel>
#include <QImage>

class PlayTable : public QLabel {
public:
    PlayTable();
    ~PlayTable();
    void Resize(QSize WinSize);

private:
    QSize min;
    QSize normal;
    QImage *mypix;
    QImage table;
};

#endif // PLAYTABLE_H
