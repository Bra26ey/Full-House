#include "gamesearchfragment.h"
#include "screenfactory.h"

#include <QLabel>
#include <QMessageBox>
using namespace screens;

GameSearchFragment::GameSearchFragment() {
    qDebug("SearchFrag create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *startVerticalContent = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;


    QLabel *title = new QLabel("Поиск комнаты");
    title->setStyleSheet("color:#242424;font-size:24px;");

    startVerticalContent->addWidget(title);
    startVerticalContent->setAlignment(Qt::AlignTop);
    buttonContainer->setAlignment(Qt::AlignBottom);

    startMainLayout->addLayout(startVerticalContent);
    startMainLayout->addLayout(buttonContainer);

    roomIdEdit = new QLineEdit;
    roomIdEdit->setMaximumWidth(335);
    roomIdEdit->setStyleSheet("color:#242424;font-size:24px");
    roomIdEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    roomIdEdit->setPlaceholderText("Id комнаты");

    passwordEdit = new QLineEdit;
    passwordEdit->setMaximumWidth(335);
    passwordEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Пароль комнаты");
    passwordEdit->setEchoMode(QLineEdit::Password);

    SearchButton = new QPushButton("Поиск");
    SearchButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SearchButton, &QPushButton::clicked, this, &GameSearchFragment::onSearchPressed);


    MenuButton = new QPushButton("Назад");
    MenuButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MenuButton, &QPushButton::clicked, this, &GameSearchFragment::onMenuPressed);

    buttonContainer->addWidget(roomIdEdit);
    buttonContainer->addWidget(passwordEdit);
    buttonContainer->addWidget(SearchButton);
    buttonContainer->addWidget(MenuButton);


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

GameSearchFragment::~GameSearchFragment() {
    delete roomIdEdit;
    delete passwordEdit;
    delete MenuButton;
    delete SearchButton;
}

int GameSearchFragment::CheckData() {
    if (roomIdEdit->text().length() < 1 || passwordEdit->text().length() < 5) {
        return 1;
    } else {
        // проверка по сетке
        return 0;
    }
}


void GameSearchFragment::onMenuPressed() {
    navigateTo(MAIN_TAG);
}

void GameSearchFragment::onSearchPressed() {
    if (CheckData()) {
        QMessageBox msgBox;
        msgBox.setText("Id and/or password incorrect");
        msgBox.setWindowTitle("Error finding room");
        msgBox.exec();
    } else {
        navigateTo(GAME_TAG);
    }
    // возможно сделать потом кейс свитч для разных ошибок
}
