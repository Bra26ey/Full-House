#ifndef GAMESEARCHFRAGMENT_H
#define GAMESEARCHFRAGMENT_H

#include "basefragment.h"

#include <QPushButton>
#include <QLineEdit>

class GameSearchFragment: public BaseFragment {
    Q_OBJECT
public:
    GameSearchFragment();
    ~GameSearchFragment();
public slots:
   void WrongDataRoomJoin();

private slots:
    void onSearchPressed();
    int CheckData();
    void onMenuPressed();
    void onHostPressed();

private:
    QLineEdit *roomIdEdit;
    QLineEdit *passwordEdit;
    QPushButton *MenuButton;
    QPushButton *SearchButton;
    QPushButton *HostButton;
};

#endif // GAMESEARCHFRAGMENT_H
