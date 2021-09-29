#include "equationsfragment.h"
#include "client_impl.h"


#include <random>
#include <QDebug>

EquationsFragment::EquationsFragment() : file(":/text/equations"), mFileslines(0) {
    std::srand(std::time(nullptr));

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *startVerticalContent = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;

    startVerticalContent->setAlignment(Qt::AlignTop);
    buttonContainer->setAlignment(Qt::AlignBottom);

    startMainLayout->addLayout(startVerticalContent);
    startMainLayout->addLayout(buttonContainer);

    TimerText = new QLabel;
    TimerText->setStyleSheet("color:#242424;font-size:24px");
    TimerText->setAlignment(Qt::AlignCenter);

    mTimer = new QTimer;
    mTimer->start(1000); // 2 минуты
    connect(mTimer, &QTimer::timeout, this, &EquationsFragment::onTimeout);

    Answer = new QLineEdit;
    Answer->setMinimumWidth(350);
    Answer->setStyleSheet("color:#242424;font-size:24px");
    Answer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QLabel *hint = new QLabel("Enter from lowest to highest solution with ;");
    Answer->setPlaceholderText("Put your answer here.");


    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()) {
        file.readLine();
        mFileslines++;
    }
    file.close();


    Equation = new QLabel;
    Answer->setMinimumWidth(350);
    Equation->setStyleSheet("color:#242424;font-size:24px;background-color:rgba(164, 191, 67, 0.6); border-radius:20px;margin:5px;padding-top:20px");
    Equation->setAlignment(Qt::AlignCenter);

    SubmitButton = new QPushButton("Submit Answer");
    SubmitButton->setStyleSheet("color:#242424;font-size:24px");
    connect(SubmitButton, &QPushButton::clicked, this, &EquationsFragment::onSubmitPressed);

    LeaveButton = new QPushButton("Exit");
    LeaveButton->setStyleSheet("color:#242424;font-size:24px");
    connect(LeaveButton, &QPushButton::clicked, this, &EquationsFragment::back);

    buttonContainer->addWidget(TimerText);
    buttonContainer->addWidget(Equation);
    buttonContainer->addWidget(hint);
    buttonContainer->addWidget(Answer);
    buttonContainer->addWidget(SubmitButton);
    buttonContainer->addWidget(LeaveButton);


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

    GenerateNewEquation();

}

EquationsFragment::~EquationsFragment() {
    delete LeaveButton;
    delete SubmitButton;
    delete Equation;
    delete Answer;
}

void EquationsFragment::onSubmitPressed() {
    if (Answer->text() == mAnswer) {
        Answer->setStyleSheet("color:#242424;font-size:24px;background-color:green");
        Client->AddMoney(time);
        Client->GetMoneyInfo();
    } else {
        Answer->setStyleSheet("color:#242424;font-size:24px;background-color:red");
    }
    GenerateNewEquation();
}

void EquationsFragment::GenerateNewEquation() {
    size_t equationsnumber = std::rand() % mFileslines - 1;
    if (equationsnumber % 2 != 0) {
        equationsnumber++;
    }
    QString equationText;
    size_t i = 0;
    file.open(QIODevice::ReadOnly);
    while (i < equationsnumber) {
        file.readLine();
        i++;
    }
    equationText = file.readLine();
    mAnswer = file.readLine();
    mAnswer.remove("\n");
    file.close();
    Equation->setText(equationText);
    time = 120;
}

void EquationsFragment::onTimeout() {
    time--;
    QTime times;
    int min = time / 60;
    int sec = time % 60;
    times.setHMS(0, min, sec);
    TimerText->setText(times.toString("mm:ss"));
    if (time == 0) {
        GenerateNewEquation();
    }
    Answer->setStyleSheet("color:#242424;font-size:24px;");
}
