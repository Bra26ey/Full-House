#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "basefragment.h"
#include "basescreensfactory.h"
#include "resolver.h"

#include <QStackedWidget>
#include <list>

class FragmentNavigator: public QObject {
    Q_OBJECT

public:
    FragmentNavigator() = delete;
    FragmentNavigator(QStackedWidget *container, BaseScreensFactory *screensFactory, Resolver* resolver);
    ~FragmentNavigator();
    BaseFragment* getStartScreen();

    void front();

public slots:
    void navigateTo(QString tag);

    void back();

    void newRootScreen(QString tag);

    void Front(BaseFragment *&current);

private:
    QStackedWidget *currentContainer;

    BaseScreensFactory *screensFactory;
    std::list<BaseFragment*> stack;

    BaseFragment* createAndConnect(QString tag);

    void connectFragment(BaseFragment *fragment);
    void disconnectFragment(BaseFragment *fragment);
};

#endif // NAVIGATOR_H
