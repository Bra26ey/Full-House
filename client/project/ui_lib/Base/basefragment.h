#ifndef BASEFRAGMENT_H
#define BASEFRAGMENT_H

#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

class BaseFragment: public QFrame {
    Q_OBJECT

signals:
    void back();

    void navigateTo(QString tag);

    void newRootScreen(QString tag);
    void replace(QString tag);

    BaseFragment* Front();

protected:
    void clearList(QLayout *list);

public:
    BaseFragment();
    ~BaseFragment();
    virtual void onPause();
    virtual void onResume();
};

#endif // BASEFRAGMENT_H
