#ifndef OTHERPLAYER_H
#define OTHERPLAYER_H

#include "card.h"
#include "chips.h"
#include "playerinfolabel.h"


#include <QPair>
#include <QLabel>
#include <QWidget>
#include <QPixmap>


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
    QPixmap *mTexture;
    size_t mTextureId;
};

#endif // OTHERPLAYER_H
