#include "loginfragment.h"

#include <boost/functional/hash/hash.hpp>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSettings>

#include "screenfactory.h"
using namespace screens;

LoginFragment::LoginFragment() {

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *startVerticalContent = new QVBoxLayout;
    QLabel *title = new QLabel("Signing in");
    QLabel *subtitle = new QLabel("Don't tell anyone authorization info.");
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;

    QVBoxLayout *buttonContainer = new QVBoxLayout;

    title->setStyleSheet("color:#242424;"
            "font-size:24px;");
    subtitle->setStyleSheet("color:#242424;"
            "font-size:24px;");;
    subtitle->setMaximumWidth(335);
    subtitle->setMinimumWidth(335);
    subtitle->setWordWrap(true);
    startVerticalContent->addWidget(title);
    startVerticalContent->addWidget(subtitle);
    startVerticalContent->setAlignment(Qt::AlignTop);
    buttonContainer->setAlignment(Qt::AlignBottom);

    startMainLayout->addLayout(startVerticalContent);
    startMainLayout->addLayout(buttonContainer);

    loginEdit->setMaximumWidth(335);
    loginEdit->setStyleSheet("color:#242424;font-size:24px");
    loginEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loginEdit->setPlaceholderText("Login");

    passwordEdit->setMaximumWidth(335);
    passwordEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Sign in");
    loginButton->setStyleSheet("color:#242424;font-size:24px");
    connect(loginButton, &QPushButton::clicked, this, &LoginFragment::onLoginPressed);

    RegistrationButton = new QPushButton("Sign up");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RegistrationButton, &QPushButton::clicked, this, &LoginFragment::onRegistrationPressed);

    quitButton = new QPushButton("Exit");
    quitButton->setStyleSheet("color:#242424;font-size:24px");
    connect(quitButton, &QPushButton::clicked, this, &LoginFragment::onExitPressed);

    buttonContainer->addWidget(loginEdit);
    buttonContainer->addWidget(passwordEdit);
    buttonContainer->addWidget(loginButton);
    buttonContainer->addWidget(RegistrationButton);
    buttonContainer->addWidget(quitButton);


    startContent->setContentsMargins(46,46,46,46);

    centerConainer->setStyleSheet("color:#242424;font-size:24px");
    centerConainer->setLayout(startContent);

    backContainer->setAlignment(Qt::AlignTop);
    startContent->addLayout(backContainer);
    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerConainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}

LoginFragment::~LoginFragment() {
    delete loginEdit;
    delete passwordEdit;
    delete loginButton;
}

void LoginFragment::onLoginPressed() {
    if (CheckData()) {
        QMessageBox msgBox;
         msgBox.setText("В пароле или логине недостаточно символов");
         msgBox.setWindowTitle("Ошибка авторизации");
         msgBox.exec();
    } else {
        navigateTo(MAIN_TAG);
    }
}

void LoginFragment::onRegistrationPressed() {
    navigateTo(REGISTRATION_TAG);
}

void LoginFragment::onExitPressed() {
    // disconnect from server
    exit(0);
}

int LoginFragment::CheckData() {
//    if (loginEdit->text().length() > 5 && passwordEdit->text().length() > 5) {
//        return 0;
//    } else {
//        return 1;
//    }
    boost::hash<std::string> PasswordHasher;
    PasswordHasher(passwordEdit->text().toStdString());
    return 0;
}

