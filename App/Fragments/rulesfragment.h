#ifndef RULESFRAGMENT_H
#define RULESFRAGMENT_H

#include "basefragment.h"
#include "screenfactory.h"

#include <QPushButton>
#include <QLabel>

class RulesFragment: public BaseFragment {
    Q_OBJECT

public:
    RulesFragment();
    ~RulesFragment();
public slots:
    void onExitPressed();

private:
    QLabel *Rules;
    QPushButton *ExitButton;
};


#endif // RULESFRAGMENT_H
