#include "registrationfragment.h"

#include <boost/functional/hash/hash.hpp>
#include <QSound>
#include <QLabel>
#include <QMessageBox>
#include "client_impl.h"

RegistrationFragment::RegistrationFragment() {
    qDebug("LoginFragment create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;


    QVBoxLayout *startVerticalContent = new QVBoxLayout;
    QLabel *title = new QLabel("Registration");
    QLabel *subtitle = new QLabel("Don't tell anyone your personal info.");
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordRepeatEdit = new QLineEdit;

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

    loginEdit->setMaximumWidth(400);
    loginEdit->setStyleSheet("color:#242424;font-size:20px");
    loginEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loginEdit->setPlaceholderText("Login");

    passwordEdit->setMaximumWidth(400);
    passwordEdit->setStyleSheet("color:#242424;font-size:20px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    passwordRepeatEdit->setMaximumWidth(400);
    passwordRepeatEdit->setStyleSheet("color:#242424;font-size:20px");
    passwordRepeatEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordRepeatEdit->setPlaceholderText("Repeat password");
    passwordRepeatEdit->setEchoMode(QLineEdit::Password);


    RegistrationButton = new QPushButton("Sign up");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RegistrationButton, &QPushButton::clicked, this, &RegistrationFragment::onRegistrationPressed);

    BackButton = new QPushButton("Back");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BackButton, &QPushButton::clicked, this, &RegistrationFragment::back);

    buttonContainer->addWidget(loginEdit);
    buttonContainer->addWidget(passwordEdit);
    buttonContainer->addWidget(passwordRepeatEdit);
    buttonContainer->addWidget(RegistrationButton);
    buttonContainer->addWidget(BackButton);

    startContent->setContentsMargins(46,46,46,46);

    centerConainer->setStyleSheet("color:#242424;font-size:24px");
    centerConainer->setLayout(startContent);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerConainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}


void RegistrationFragment::WrongDataRegistration() {
    QMessageBox msgBox;
    msgBox.setText("Info is incorrect");
    msgBox.setWindowTitle("Registration error");
    msgBox.exec();
}

void RegistrationFragment::RightDataRegistration() {
    QMessageBox msgBox;
    msgBox.setText("Registration succesfully done");
    msgBox.setWindowTitle("Success");
    msgBox.exec();
}

void RegistrationFragment::onRegistrationPressed() {
    QSound::play(":/music/click");
    if (CheckData() == 2) {
        QMessageBox msgBox;
        msgBox.setText("Password and/or login doesn't have enough characters");
        msgBox.setWindowTitle("Registration error");
        msgBox.exec();
    } else if (CheckData() == 1) {
        QMessageBox msgBox;
        msgBox.setText("Passwords don't match");
        msgBox.setWindowTitle("Registration error");
        msgBox.exec();
    } else {
        boost::hash<std::string> PasswordHasher;
        PasswordHasher(passwordEdit->text().toStdString());
        Client->Registrate(loginEdit->text().toStdString(), passwordEdit->text().toStdString());
    }
}

int RegistrationFragment::CheckData() {
    if (passwordEdit->text() != passwordRepeatEdit->text()) {
        return 1;
    } else if (loginEdit->text().length() > 5 && passwordEdit->text().length() > 5) {
        return 0;
    }
    return 2;
}

RegistrationFragment::~RegistrationFragment() {
    delete loginEdit;
    delete passwordEdit;
    delete passwordRepeatEdit;
    delete RegistrationButton;
}
