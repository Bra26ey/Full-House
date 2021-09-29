#include "creditmenufragment.h"

#include <QSound>
#include <QMessageBox>

using namespace screens;

CreditMenuFragment::CreditMenuFragment() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;


    EquationsButton = new QPushButton("Equation Solver");
    EquationsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(EquationsButton, &QPushButton::clicked, this, &CreditMenuFragment::onEquationsPressed);

    MiniGamesButton = new QPushButton("MiniGames (under development)");
    MiniGamesButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MiniGamesButton, &QPushButton::clicked, this, &CreditMenuFragment::onMiniGamesPressed);

    MinerButton = new QPushButton("Mine credits");
    MinerButton->setStyleSheet("color:#242424;font-size:24px");
    connect(MinerButton, &QPushButton::clicked, this, &CreditMenuFragment::onMinerPressed);

    LeaveButton = new QPushButton("Return");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &CreditMenuFragment::back);

    buttonContainer->addWidget(EquationsButton);
    buttonContainer->addWidget(MiniGamesButton);
    buttonContainer->addWidget(MinerButton);
    buttonContainer->addWidget(LeaveButton);

    loadingButtonContainer->addWidget(EquationsButton);
    loadingButtonContainer->addWidget(MiniGamesButton);
    loadingButtonContainer->addWidget(MinerButton);
    loadingButtonContainer->addWidget(LeaveButton);

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

CreditMenuFragment::~CreditMenuFragment() {
    delete EquationsButton;
    delete MiniGamesButton;
    delete MinerButton;
    delete LeaveButton;
}

void CreditMenuFragment::onMiniGamesPressed() {
    QSound::play(":/music/click");
}

void CreditMenuFragment::onEquationsPressed() {
    QSound::play(":/music/click");
    navigateTo(EQUATION_TAG);
}

void CreditMenuFragment::onMinerPressed() {
    QSound::play(":/music/click");
    QMessageBox msgBox;
     msgBox.setText("Just kidding, there are no viruses 😉");
     msgBox.setWindowTitle("BITCOIN MINER HAS BEEN INSTALLED");
     msgBox.exec();
}

