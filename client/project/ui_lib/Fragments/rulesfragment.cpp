#include "rulesfragment.h"

#include <QFile>
#include <QSound>

RulesFragment::RulesFragment() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QVBoxLayout *controlButtons = new QVBoxLayout;


    ControlButton = new QPushButton("Next Page");
    ControlButton->setStyleSheet("color:#242424;font-size:24px");
    connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);


    ExitButton = new QPushButton("Back");
    ExitButton->setStyleSheet("color:#242424;font-size:24px");
    connect(ExitButton, &QPushButton::clicked, this, &RulesFragment::onExitPressed);

    Rules1 = new QLabel;
    QFile file(":/text/rules1");
    file.open(QIODevice::ReadOnly);
    Rules1->setText(file.readAll());
    file.close();

    Rules2 = new QLabel;
    QFile file2(":/text/rules2");
    file2.open(QIODevice::ReadOnly);
    Rules2->setText(file2.readAll());
    file2.close();

    Rules1->setStyleSheet("font-family: sans-serif;font-size:20px;background-color:rgba(164, 191, 67, 0.6); border-radius:20px;margin:5px");
    Rules2->setStyleSheet("font-family: sans-serif;font-size:20px;background-color:rgba(164, 191, 67, 0.6); border-radius:20px;margin:5px");
    Rules2->hide();
    controlButtons->addWidget(ControlButton);
    buttonContainer->addWidget(ExitButton);

    startContent->setContentsMargins(46,46,46,46);
    startContent->addWidget(Rules1);
    startContent->addWidget(Rules2);

    centerConainer->setStyleSheet("color:#242424;font-size:24px");
    centerConainer->setLayout(startContent);

    backContainer->setAlignment(Qt::AlignTop);
    startContent->addLayout(backContainer);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerConainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->addLayout(controlButtons);
    mainVLayout->addLayout(buttonContainer);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}

void RulesFragment::onNextPressed() {
    QSound::play(":/music/click");
    ControlButton->disconnect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);
    ControlButton->connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onPrevPressed);
    ControlButton->setText("Previous Page");
    Rules1->hide();
    Rules2->show();
}

void RulesFragment::onPrevPressed() {
    QSound::play(":/music/click");
    ControlButton->disconnect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onPrevPressed);
    ControlButton->connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);
    ControlButton->setText("Next Page");
    Rules2->hide();
    Rules1->show();
}

void RulesFragment::onExitPressed() {
    QSound::play(":/music/click");
    back();
}

RulesFragment::~RulesFragment() {
    delete Rules1;
    delete Rules2;
    delete ControlButton;
    delete ExitButton;
}
