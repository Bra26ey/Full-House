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
    void onSearchPressed();
    void CheckData();
    void onMenuPressed();

private:
    QLineEdit *roomIdEdit;
    QLineEdit *passwordEdit;
    QPushButton *MenuButton;
    QPushButton *SearchButton;
};

#endif // GAMESEARCHFRAGMENT_H
