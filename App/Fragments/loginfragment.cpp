#include "loginfragment.h"

#include <QLabel>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSettings>

#include "screenfactory.h"
using namespace screens;

LoginFragment::LoginFragment() {
    qDebug("LoginFragment create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *startVerticalContent = new QVBoxLayout;
    QLabel *title = new QLabel("Вход в приложение");
    QLabel *subtitle = new QLabel("Не сообщайте никому свои данные для авторизации.");
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;

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

    loginEdit->setMaximumWidth(335);
    loginEdit->setStyleSheet("color:#242424;font-size:24px");
    loginEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loginEdit->setPlaceholderText("Логин");

    passwordEdit->setMaximumWidth(335);
    passwordEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Пароль");
    passwordEdit->setEchoMode(QLineEdit::Password);
    connect(loginEdit, &QLineEdit::textChanged, this, &LoginFragment::CheckData);
    connect(passwordEdit, &QLineEdit::textChanged, this, &LoginFragment::CheckData);

    loginButton = new QPushButton("Войти");
    loginButton->setStyleSheet("color:#242424;font-size:24px");
    connect(loginButton, &QPushButton::clicked, this, &LoginFragment::onLoginPressed);

    RegistrationButton = new QPushButton("Регистрация");
    RegistrationButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RegistrationButton, &QPushButton::clicked, this, &LoginFragment::onRegistrationPressed);

    buttonContainer->addWidget(loginEdit);
    buttonContainer->addWidget(passwordEdit);
    loadingButtonContainer->addWidget(loginButton);
    loadingButtonContainer->addWidget(RegistrationButton);
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

int LoginFragment::CheckData() {
    if (loginEdit->text().length() > 5 && passwordEdit->text().length() > 5) {
        return 0;
    } else {
        return 1;
    }
}

