#ifndef OTHERPLAYER_H
#define OTHERPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QPair>


class OtherPlayer: public QWidget {
        Q_OBJECT
public:
    OtherPlayer();
    virtual ~OtherPlayer();
    void DisplayInfo();
    void DisplayPlayer();

protected:
    QWidget *Chips;
    QPair<QWidget, QWidget> *Cards;
    QLabel *PlayerInfo;

private:
    size_t TextureId;
};

#endif // OTHERPLAYER_H
