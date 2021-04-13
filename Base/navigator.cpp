#include "navigator.h"
#include "screenfactory.h"

#include <QLinkedList>

using namespace screens;

FragmentNavigator::FragmentNavigator(
        QStackedWidget *container,
        BaseScreensFactory *screensFactory) {

    qDebug("create navigator");
    this->screensFactory = screensFactory;
    this->currentContainer = container;
    qDebug("create start fragment");
    BaseFragment* startFragment = getStartScreen();
    qDebug("append start fragment");
    this->stack.append(startFragment);

    qDebug("add widget");
    currentContainer->addWidget(stack.last());
    currentContainer->setCurrentIndex(0);
}

FragmentNavigator::~FragmentNavigator() {
}


void FragmentNavigator::navigateTo(QString tag) {
    qDebug("Navigator navigateTo");
    BaseFragment *newFragment = this->screensFactory->create(tag);
    stack.last()->onPause();
    disconnectFragment(stack.last());
    connectFragment(newFragment);
    stack.append(newFragment);
    currentContainer->addWidget(newFragment);
    currentContainer->setCurrentWidget(newFragment);
}

void FragmentNavigator::back() {
    qDebug("Navigator back");
    currentContainer->removeWidget(stack.last());
    delete stack.last();
    stack.removeLast();
    connectFragment(stack.last());
    stack.last()->onResume();
    currentContainer->setCurrentWidget(stack.last());
}

void FragmentNavigator::replace(QString tag) {
    qDebug("Navigator replace");
    BaseFragment *newFragment = this->screensFactory->create(tag);
    currentContainer->removeWidget(stack.last());
    delete stack.last();
    stack.removeLast();
    connectFragment(newFragment);
    currentContainer->addWidget(newFragment);
}

void FragmentNavigator::newRootScreen(QString tag) {
    qDebug("Navigator newRootScreen");
    BaseFragment *newFragment = this->screensFactory->create(tag);
    disconnectFragment(stack.last());
    stack.clear();
    connectFragment(newFragment);
    for(int i = currentContainer->count(); i >= 0; i--)
    {
        QWidget* widget = currentContainer->widget(i);
        currentContainer->removeWidget(widget);
        widget->deleteLater();
    }
    currentContainer->addWidget(newFragment);
    stack.append(newFragment);
}


BaseFragment* FragmentNavigator::getStartScreen() {
    return createAndConnect(this->screensFactory->createStart());
}

void FragmentNavigator::connectFragment(BaseFragment *fragment) {
    qDebug("Navigator connect slots");
    connect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    connect(fragment, &BaseFragment::replace, this, &FragmentNavigator::replace);
    connect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    connect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
}

void FragmentNavigator::disconnectFragment(BaseFragment *fragment) {
    qDebug("Navigator disconnect slots");
    disconnect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    disconnect(fragment, &BaseFragment::replace, this, &FragmentNavigator::replace);
    disconnect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    disconnect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
}

BaseFragment* FragmentNavigator::createAndConnect(QString tag) {
    BaseFragment *fragment = this->screensFactory->create(tag);
    qDebug("Navigator create screen");
    connectFragment(fragment);
    return fragment;
}
