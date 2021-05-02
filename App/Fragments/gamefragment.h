#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"
#include "otherplayer.h"
#include "playtable.h"
#include "card.h"
#include "dealerlogo.h"

#include <QVector>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>

class GameFragment: public BaseFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();
    void DrawPlayer(QRect pos, size_t player_id, std::string nickname, size_t total_money);
    void MakeDealer(size_t player_id);

public slots:
    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();

    void onLeavePressed();
    void onSettingsPressed();

private slots:
    void setval();

private:
    size_t num_players;

    QLabel *mPlayer;
    QVector<OtherPlayer*> OtherPlayers;
    QLabel *mPlayTable;
    QLabel *mDealerLogo;

    QVector <Card*> CardOnTable;

    QSlider *BetSlider;
    QLineEdit *BetValue;

    QPushButton *BetButton;
    QPushButton *RaiseButton;
    QPushButton *FoldButton;
    QPushButton *CheckButton;

    QPushButton *LeaveButton;
    QPushButton *SettingsButton;
};

#endif // GAMEFRAGMENT_H
