#ifndef OTHERPLAYER_H
#define OTHERPLAYER_H

#include "card.h"
#include "chips.h"
#include "playerinfolabel.h"


#include <QPair>
#include <QLabel>
#include <QWidget>
#include <QPixmap>


class OtherPlayer: public QLabel {
        Q_OBJECT
public:
    OtherPlayer(size_t id);
    ~OtherPlayer();
    void DisplayInfo();
    void DisplayPlayer();
    void SetPosition(QRect &pos);

protected:
    QWidget *Chips;
    QPair<Card, Card> *Cards;
    QLabel *PlayerInfo;


private:
    QRect mPos;
    QImage *mypix;
    QImage player;
    size_t mTextureId;
};

#endif // OTHERPLAYER_H
