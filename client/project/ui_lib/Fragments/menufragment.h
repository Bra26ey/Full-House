#ifndef MENUFRAGMENT_H
#define MENUFRAGMENT_H

#include "basefragment.h"
#include "screenfactory.h"

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
    void onRulesPressed();
    void onCreditsPressed();

private:
    QPushButton *LoginButton;
    QPushButton *SettingsButton;
    QPushButton *CreditsButton;
    QPushButton *RulesButton;
    QPushButton *SearchButton;
};

#endif // MENUFRAGMENT_H
