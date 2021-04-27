#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"
#include "playtable.h"
#include "card.h"

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
    QLabel *Player;
    QVector<QLabel*> OtherPlayers;
    QLabel *playtable;

    QVector <Card*> cards_on_table;

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
