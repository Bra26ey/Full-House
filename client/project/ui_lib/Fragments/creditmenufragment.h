#ifndef CREDITMENU_H
#define CREDITMENU_H

#include "basefragment.h"
#include "screenfactory.h"

#include <QPushButton>

class CreditMenuFragment: public BaseFragment {
public:
    CreditMenuFragment();
    ~CreditMenuFragment();
public slots:
    void onEquationsPressed();
    void onMiniGamesPressed();
    void onMinerPressed();
private:
    QPushButton *EquationsButton;
    QPushButton *MiniGamesButton;
    QPushButton *MinerButton;
    QPushButton *LeaveButton;
};

#endif // CREDITMENU_H
