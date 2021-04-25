#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"
#include "playtable.h"

#include <QVector>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

class GameFragment: public BaseFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();

public slots:

    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();

    void onLeavePressed();
    void onSettingsPressed();

private:
    QLabel *Player;
    QVector<QLabel*> OtherPlayers;
    QLabel *playtable;

    QSlider *BetSlider;
    QLabel *BetValue;

    QPushButton *BetButton;
    QPushButton *RaiseButton;
    QPushButton *FoldButton;
    QPushButton *CheckButton;

    QPushButton *LeaveButton;
    QPushButton *SettingsButton;
};

#endif // GAMEFRAGMENT_H
