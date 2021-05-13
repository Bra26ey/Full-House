#include "menufragment.h"
#include "client_impl.h"

#include <QSound>

using namespace screens;

MenuFragment::MenuFragment() {
    qDebug("MenuFragment create");

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;


    LoginButton = new QPushButton("Log out");
    LoginButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LoginButton, &QPushButton::clicked, this, &MenuFragment::onLoginPressed);

    RulesButton = new QPushButton("Rules");
    RulesButton->setStyleSheet("color:#242424;font-size:24px");
    connect(RulesButton, &QPushButton::clicked, this, &MenuFragment::onRulesPressed);

    SettingsButton = new QPushButton("Settings");
    SettingsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SettingsButton, &QPushButton::clicked, this, &MenuFragment::onSettingsPressed);

    SearchButton = new QPushButton("Search game");
    SearchButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SearchButton, &QPushButton::clicked, this, &MenuFragment::onSearchPressed);

    buttonContainer->addWidget(SearchButton);
    buttonContainer->addWidget(RulesButton);
    buttonContainer->addWidget(SettingsButton);
    buttonContainer->addWidget(LoginButton);

    loadingButtonContainer->addWidget(SearchButton);
    loadingButtonContainer->addWidget(RulesButton);
    loadingButtonContainer->addWidget(SettingsButton);
    loadingButtonContainer->addWidget(LoginButton);

    buttonContainer->addLayout(loadingButtonContainer);

    startMainLayout->addLayout(buttonContainer);
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

MenuFragment::~MenuFragment() {
    delete LoginButton;
    delete SettingsButton;
    delete SearchButton;
    delete RulesButton;
}

void MenuFragment::onSearchPressed() {
    QSound::play(":/music/click");
    navigateTo(SEARCH_TAG);
}

void MenuFragment::onSettingsPressed() {
    QSound::play(":/music/click");
    navigateTo(SETTINGS_TAG);
}

void MenuFragment::onLoginPressed() {
    QSound::play(":/music/click");
    Client->Logout(); // выход не залогинненого юзера все крашит
    navigateTo(LOGIN_TAG);
}

void MenuFragment::onRulesPressed() {
    QSound::play(":/music/click");
    navigateTo(RULE_TAG);
}
