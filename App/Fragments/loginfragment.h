#ifndef LOGINFRAGMENT_H
#define LOGINFRAGMENT_H

#include "basefragment.h"

#include <QLineEdit>
#include <QPushButton>


class LoginFragment: public BaseFragment {
    Q_OBJECT

public:
    LoginFragment();
    ~LoginFragment();
public slots:
    void onLoginPressed();
    void onRegistrationPressed();
    int CheckData();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;

    QPushButton *RegistrationButton;
};

#endif // LOGINFRAGMENT_H
