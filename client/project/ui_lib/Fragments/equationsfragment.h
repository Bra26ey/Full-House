#ifndef EQUATIONSFRAGMENT_H
#define EQUATIONSFRAGMENT_H

#include "basefragment.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QString>
#include <QTimer>
#include <QTime>

class EquationsFragment : public BaseFragment {
public:
    EquationsFragment();
    ~EquationsFragment();

    void GenerateNewEquation();

public slots:
    void onSubmitPressed();
    void onTimeout();

private:
    size_t time = 120;
    QLabel* TimerText;
    QTimer* mTimer;
    QString mAnswer;
    QFile file;
    size_t mFileslines;
    QPushButton* LeaveButton;
    QPushButton* SubmitButton;
    QLabel* Equation;
    QLineEdit* Answer;
};

#endif // EQUATIONSFRAGMENT_H
