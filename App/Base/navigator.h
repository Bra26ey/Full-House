#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "basefragment.h"
#include "basescreensfactory.h"

#include <QStackedWidget>
#include <list>

class FragmentNavigator: public QObject {
    Q_OBJECT

public:
    FragmentNavigator() = delete;
    FragmentNavigator(QStackedWidget *container, BaseScreensFactory *screensFactory);
    ~FragmentNavigator();
    BaseFragment* getStartScreen();

public slots:
    void navigateTo(QString tag);

    void back();

    void replace(QString tag);

    void newRootScreen(QString tag);

private:
    QStackedWidget *currentContainer;

    BaseScreensFactory *screensFactory;
    std::list<BaseFragment*> stack;

    BaseFragment* createAndConnect(QString tag);

    void connectFragment(BaseFragment *fragment);
    void disconnectFragment(BaseFragment *fragment);
};

#endif // NAVIGATOR_H
