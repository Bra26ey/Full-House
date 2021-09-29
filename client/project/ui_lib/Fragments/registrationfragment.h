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
    void WrongDataRegistration();
    void RightDataRegistration();

private slots:
    void onRegistrationPressed();
    int CheckData();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passwordRepeatEdit;

    QPushButton *RegistrationButton;
    QPushButton *BackButton;
};

#endif // REGISTRATIONFRAGMENT_H
