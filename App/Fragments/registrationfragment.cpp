#include "registrationfragment.h"

#include <QLabel>
#include <QMessageBox>

RegistrationFragment::RegistrationFragment() {
    qDebug("LoginFragment create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;


    QVBoxLayout *startVerticalContent = new QVBoxLayout;
    QLabel *title = new QLabel("Регистрация");
    QLabel *subtitle = new QLabel("Не сообщайте никому свои данные для регистрации.");
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordRepeatEdit = new QLineEdit;
    emailEdit = new QLineEdit;

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

    emailEdit->setMaximumWidth(400);
    emailEdit->setStyleSheet("color:#242424;font-size:20px");
    emailEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    emailEdit->setPlaceholderText("Email");


    RegistrationButton = new QPushButton("Sign up");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RegistrationButton, &QPushButton::clicked, this, &RegistrationFragment::onRegistrationPressed);

    BackButton = new QPushButton("Sign in");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BackButton, &QPushButton::clicked, this, &RegistrationFragment::back);

    buttonContainer->addWidget(loginEdit);
    buttonContainer->addWidget(emailEdit);
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



void RegistrationFragment::onRegistrationPressed() {
    if (CheckData()) {
        QMessageBox msgBox;
         msgBox.setText("В пароле или логине недостаточно символов");
         msgBox.setWindowTitle("Ошибка регистрации");
         msgBox.exec();
    } else {
        // send data to database
    }
}

int RegistrationFragment::CheckData() {
    if (loginEdit->text().length() > 5 && passwordEdit->text().length() > 5) {
        return 0;
    } else {
        return 1;
    }
}

RegistrationFragment::~RegistrationFragment() {
    delete loginEdit;
    delete passwordEdit;
    delete passwordRepeatEdit;
    delete emailEdit;

    delete RegistrationButton;
}
