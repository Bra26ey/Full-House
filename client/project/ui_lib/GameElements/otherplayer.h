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
static QRect secondpos(110, 30, 400, 400);         // 1     5
static QRect thirdpos(470, -20, 400, 400);        //     0
static QRect fouthpos(850, 30, 400, 400);
static QRect fifthpos(900, 480, 400, 400);
static QRect mainplayerpos(500, 590, 400, 400);

static QRect firstposmin(60, 420, 400, 400);           // 2   4
static QRect secondposmin(90, 0, 400, 400);         // 1     5
static QRect thirdposmin(390, -40, 400, 400);        //     0
static QRect fouthposmin(700, 00, 400, 400);
static QRect fifthposmin(740, 390, 400, 400);
static QRect mainplayerposmin(400, 470, 400, 400);

//TODO refactor


class OtherPlayer: public QLabel {
        Q_OBJECT
public:
    OtherPlayer() = default;
    OtherPlayer(size_t id, std::string nickname, size_t money);
    ~OtherPlayer();
    void SetPosition(QRect &pos);

    // card interface
    void GiveCards(size_t value1, size_t suit1, size_t value2, size_t suit2);
    void DiscardCards();
    void FlipCards();


    size_t GetBet();
    bool GetCardSide();
    bool GetHasCards();

    //status interface
    void setRaise(size_t bet);
    void setFold();
    void setCheck();
    void setCall();
    void ClearStatus();
    void setBet(size_t bet);
    void setMoney(size_t money);

    QString GetName();
    void AddMoney(size_t add);

    virtual void Resize(QSize WinSize);
    QRect GetPos();

    QPair<Card*, Card*> GetCard();


protected:
    void setCardPos();
    QSize normal;
    QSize min;
    QPair<Card*, Card*> mCards;
    PlayerInfoLabel *mPlayerInfo;

    QRect mPos;
    QImage *mypix;
    QImage player;
    size_t mTextureId;
private:
    bool HasCards;
    void posChange();
    bool LowRes;
};

#endif // OTHERPLAYER_H
