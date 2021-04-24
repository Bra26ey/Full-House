#ifndef REGISTRATIONFRAGMENT_H
#define REGISTRATIONFRAGMENT_H

#include "basefragment.h"

#include <QLineEdit>
#include <QPushButton>

class RegistrationFragment: public BaseFragment {
    Q_OBJECT

public:
    RegistrationFragment();
    ~RegistrationFragment();

public slots:
    void onRegistrationPressed();
    void checkData();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passwordRepeatEdit;
    QLineEdit *emailEdit;

    QPushButton *RegistrationButton;
    QPushButton *BackButton;
};

#endif // REGISTRATIONFRAGMENT_H