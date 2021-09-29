#include "gamesearchfragment.h"
#include "screenfactory.h"
#include "client_impl.h"

#include <QLabel>
#include <QSound>
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


    QLabel *title = new QLabel("Search room");
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
    roomIdEdit->setPlaceholderText("Room ID");

    passwordEdit = new QLineEdit;
    passwordEdit->setMaximumWidth(335);
    passwordEdit->setStyleSheet("color:#242424;font-size:24px");
    passwordEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordEdit->setPlaceholderText("Room Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    SearchButton = new QPushButton("Search");
    SearchButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SearchButton, &QPushButton::clicked, this, &GameSearchFragment::onSearchPressed);


    MenuButton = new QPushButton("Back");
    MenuButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MenuButton, &QPushButton::clicked, this, &GameSearchFragment::onMenuPressed);

    HostButton = new QPushButton("Host");
    HostButton->setStyleSheet("color:#242424;font-size:24px");
    connect(HostButton, &QPushButton::clicked, this, &GameSearchFragment::onHostPressed);

    buttonContainer->addWidget(roomIdEdit);
    buttonContainer->addWidget(passwordEdit);
    buttonContainer->addWidget(SearchButton);
    buttonContainer->addWidget(HostButton);
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
    if (passwordEdit->text().length() < 5) {
        return 1;
    } else {
        return 0;
    }
}

void GameSearchFragment::WrongDataRoomJoin() {
    QMessageBox msgBox;
    msgBox.setText("Cannot join to room");
    msgBox.setWindowTitle("Joining error");
    msgBox.exec();
}

void GameSearchFragment::onMenuPressed() {
    QSound::play(":/music/click");
    back();
}

void GameSearchFragment::onHostPressed() {
    QSound::play(":/music/click");
    if (CheckData()) {
        QMessageBox msgBox;
        msgBox.setText("Id and/or password incorrect");
        msgBox.setWindowTitle("Error Hosting room");
        msgBox.exec();
    } else if ( globalInfo::Balance < 2) {
        QMessageBox msgBox;
        msgBox.setText("Go find some money, buddy");
        msgBox.setWindowTitle("Error Hosting room");
        msgBox.exec();
    } else {
         Client->CreateRoom(passwordEdit->text().toStdString());
         globalInfo::Password = passwordEdit->text().toStdString();
    }
}

void GameSearchFragment::onSearchPressed() {
    QSound::play(":/music/click");
    if (CheckData()) {
        QMessageBox msgBox;
        msgBox.setText("Id and/or password incorrect");
        msgBox.setWindowTitle("Error finding room");
        msgBox.exec();
    } else if ( globalInfo::Balance < 2) {
        QMessageBox msgBox;
        msgBox.setText("Go find some money, buddy");
        msgBox.setWindowTitle("Error finding room");
        msgBox.exec();
    } else {
        Client->JoinRoom(roomIdEdit->text().toULongLong(), passwordEdit->text().toStdString());
        globalInfo::RoomId = roomIdEdit->text().toULongLong();
        globalInfo::Password = passwordEdit->text().toStdString();
    }
    // возможно сделать потом кейс свитч для разных ошибок
}
