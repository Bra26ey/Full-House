#ifndef LOGINFRAGMENT_H
#define LOGINFRAGMENT_H

#include "basefragment.h"
#include "client_impl.h"

#include <QLineEdit>
#include <QPushButton>


class LoginFragment: public BaseFragment {
    Q_OBJECT

public:
    LoginFragment();
    ~LoginFragment();
public slots:
    void WrongData();
private slots:
    void onLoginPressed();
    void onExitPressed();
    void onRegistrationPressed();
    int CheckData();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;

    QPushButton *loginButton;
    QPushButton *RegistrationButton;
    QPushButton *quitButton;
};

#endif // LOGINFRAGMENT_H
