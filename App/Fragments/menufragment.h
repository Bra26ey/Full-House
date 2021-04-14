#ifndef MENUFRAGMENT_H
#define MENUFRAGMENT_H

#include "basefragment.h"

#include <QPushButton>

class MenuFragment: public BaseFragment {
    Q_OBJECT

public:
    MenuFragment();
    ~MenuFragment();
public slots:
    void onLoginPressed();
    void onSettingsPressed();
    void onSearchPressed();

private:
    QPushButton *LoginButton;
    QPushButton *SettingsButton;
    QPushButton *SearchButton;
};

#endif // MENUFRAGMENT_H
