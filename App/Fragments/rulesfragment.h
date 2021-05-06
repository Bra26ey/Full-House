#ifndef RULESFRAGMENT_H
#define RULESFRAGMENT_H

#include "basefragment.h"
#include "screenfactory.h"

#include <QPushButton>

class RulesFragment: public RulesFragment {
    Q_OBJECT

public:
    RulesFragment();
    ~RulesFragment();
public slots:
    void onLoginPressed();
    void onSettingsPressed();
    void onSearchPressed();

private:
    QPushButton *ExitButton;
};

#endif // MENUFRAGMENT_H

#endif // RULESFRAGMENT_H
