#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"

#include <QVector>
#include <QWidget>
#include <QPushButton>

class GameFragment: public BaseFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();

public slots:
    void ChangedSlider();

    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();

    void onLeavePressed();
    void onSettingsPressed();

private:
    QWidget *Player;
    QVector<QWidget> *OtherPlayers;
    QWidget *PlayTable;

    QPushButton *BetButton;
    QPushButton *RaiseButton;
    QPushButton *FoldButton;
    QPushButton *CheckButton;

    QPushButton *LeaveButton;
    QPushButton *SettingsButton;
};

#endif // GAMEFRAGMENT_H
