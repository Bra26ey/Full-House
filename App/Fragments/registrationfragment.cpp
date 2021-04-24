#include "registrationfragment.h"

#include <QLabel>

RegistrationFragment::RegistrationFragment() {
    qDebug("LoginFragment create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *startVerticalContent = new QVBoxLayout;
    QLabel *title = new QLabel("Регистрация");
    QLabel *subtitle = new QLabel("Не сообщайте никому свои данные для регистрации.");
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordRepeatEdit = new QLineEdit;
    emailEdit = new QLineEdit;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;

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
    loginEdit->setStyleSheet("color:#242424;font-size:24px");
    loginEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loginEdit->setPlaceholderText("Логин");

    passwordEdit->setMaximumWidth(400);
    passwordEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Пароль");
    passwordEdit->setEchoMode(QLineEdit::Password);

    passwordRepeatEdit->setMaximumWidth(400);
    passwordRepeatEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordRepeatEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordRepeatEdit->setPlaceholderText("Повторите введенный пароль");
    passwordRepeatEdit->setEchoMode(QLineEdit::Password);

    emailEdit->setMaximumWidth(400);
    emailEdit->setStyleSheet("color:#242424;font-size:24px");
    emailEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    emailEdit->setPlaceholderText("Электронная почта");


    RegistrationButton = new QPushButton("Регистрация");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RegistrationButton, &QPushButton::clicked, this, &RegistrationFragment::onRegistrationPressed);

    BackButton = new QPushButton("Назад к окну авторизации");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BackButton, &QPushButton::clicked, this, &RegistrationFragment::back);

    buttonContainer->addWidget(loginEdit);
    buttonContainer->addWidget(emailEdit);
    buttonContainer->addWidget(passwordEdit);
    buttonContainer->addWidget(passwordRepeatEdit);
    loadingButtonContainer->addWidget(RegistrationButton);
    loadingButtonContainer->addWidget(BackButton);
    buttonContainer->addLayout(loadingButtonContainer);

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



void RegistrationFragment::onRegistrationPressed() {

}

void RegistrationFragment::checkData() {

}

RegistrationFragment::~RegistrationFragment() {
    delete loginEdit;
    delete passwordEdit;
    delete passwordRepeatEdit;
    delete emailEdit;

    delete RegistrationButton;
}
