#ifndef OTHERPLAYER_H
#define OTHERPLAYER_H

#include "card.h"
#include "chips.h"
#include "playerinfolabel.h"


#include <QPair>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
                                                      //  3
static QRect firstpos(70, 480, 400, 400);           // 2   4
static QRect secondpos(110, 30, 400, 400);           // 1     5
static QRect thirdpos(470, -20, 400, 400);        //     0
static QRect fouthpos(850, 30, 400, 400);
static QRect fifthpos(900, 480, 400, 400);

//TODO refactor


class OtherPlayer: public QLabel {
        Q_OBJECT
public:
    OtherPlayer(size_t id);
    ~OtherPlayer();
    void SetPosition(QRect &pos);

    // card interface
    void GiveCards(size_t value1, size_t suit1, size_t value2, size_t suit2);
    void DiscardCards();
    void FlipCards();

    void DisplayInfo();
protected:
    QWidget *Chips;
    QPair<Card*, Card*> mCards;
    QLabel *PlayerInfo;


private:
    QRect mPos;
    QImage *mypix;
    QImage player;
    size_t mTextureId;
};

#endif // OTHERPLAYER_H
