#include "loginfragment.h"

#include <boost/functional/hash/hash.hpp>
#include <QApplication>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

#include <QSound>

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
    QSound::play(":/music/click");
    if (CheckData()) {
        QMessageBox msgBox;
         msgBox.setText("Password and/or login doesn't have enough characters");
         msgBox.setWindowTitle("Autorisation error");
         msgBox.exec();
    } else {
        boost::hash<std::string> PasswordHasher;
        PasswordHasher(passwordEdit->text().toStdString()); 
        Client->Autorise(loginEdit->text().toStdString(), passwordEdit->text().toStdString());
        globalInfo::Nickname = loginEdit->text().toStdString();
        Client->GetMoneyInfo();
    }
}

void LoginFragment::onRegistrationPressed() {
    QSound::play(":/music/click");
    navigateTo(REGISTRATION_TAG);
}

void LoginFragment::onExitPressed() {
    QSound::play(":/music/click");
    QApplication::quit();
}

void LoginFragment::WrongData() {
    QMessageBox msgBox;
    msgBox.setText("Info is incorrect");
    msgBox.setWindowTitle("Autorisation error");
    msgBox.exec();
}

int LoginFragment::CheckData() {
    if (loginEdit->text().length() < 5 && passwordEdit->text().length() < 5) {
        return 1;
    }
    return 0;
}
