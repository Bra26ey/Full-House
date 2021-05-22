#include "navigator.h"
#include "screenfactory.h"



using namespace screens;

FragmentNavigator::FragmentNavigator(
        QStackedWidget *container,
        BaseScreensFactory *screensFactory,
        Resolver* resolver) : mResolver(resolver) {

    qDebug("create navigator");
    this->screensFactory = screensFactory;
    this->currentContainer = container;
    qDebug("create start fragment");
    BaseFragment* startFragment = getStartScreen();
    qDebug("append start fragment");
    connectFragment(resolver);
    this->stack.push_back(startFragment);

    qDebug("add widget");
    currentContainer->addWidget(stack.back());
    currentContainer->setCurrentIndex(0);
}

FragmentNavigator::~FragmentNavigator() {
}


void FragmentNavigator::navigateTo(QString tag) {
    qDebug("Navigator navigateTo");
    BaseFragment *newFragment = this->screensFactory->create(tag);
    stack.back()->onPause();
    disconnectFragment(stack.back());
    connectFragment(newFragment);
    stack.push_back(newFragment);
    if (tag == GAME_TAG) {
        GameFragment* game = static_cast<GameFragment*>(newFragment);
        connect(mResolver, &Resolver::DeletePlayer, game, &GameFragment::DeletePlayer);
        connect(mResolver, &Resolver::DrawPlayer, game, &GameFragment::DrawPlayer);
        connect(mResolver, &Resolver::SetMinBet, game, &GameFragment::SetMinBet);
        connect(mResolver, &Resolver::SetMaxBet, game, &GameFragment::SetMaxBet);

        connect(mResolver, &Resolver::EndGame, game, &GameFragment::EndGame);
        connect(mResolver, &Resolver::JoinNotAdmin, game, &GameFragment::JoinNotAdmin);
        connect(mResolver, &Resolver::FlipTableCards, game, &GameFragment::FlipTableCards);
        connect(mResolver, &Resolver::DeleteAllCardsFromTable, game, &GameFragment::DeleteAllCardsFromTable);

        connect(mResolver, &Resolver::AddCardToTable, game, &GameFragment::AddCardToTable);
        connect(mResolver, &Resolver::FlipAllCards, game, &GameFragment::FlipAllCards);


        connect(mResolver, &Resolver::ShowActions, game, &GameFragment::ShowActions);
        connect(mResolver, &Resolver::BlockActions, game, &GameFragment::BlockActions);
        connect(mResolver, &Resolver::UnBlockActions, game, &GameFragment::UnBlockActions);
        connect(mResolver, &Resolver::HideStart, game, &GameFragment::HideStart);

        connect(mResolver, &Resolver::MakeDealer, game, &GameFragment::MakeDealer);
        connect(mResolver, &Resolver::DisplayWinner, game, &GameFragment::DisplayWinner);
        connect(mResolver, &Resolver::CurrentTurn, game, &GameFragment::CurrentTurn);
        connect(mResolver, &Resolver::GiveCards, game, &GameFragment::GiveCards);

        connect(mResolver, &Resolver::FlipCards, game, &GameFragment::FlipCards);
        connect(mResolver, &Resolver::SetBet, game, &GameFragment::SetBet);
        connect(mResolver, &Resolver::SetFold, game, &GameFragment::SetFold);
        connect(mResolver, &Resolver::SetCall, game, &GameFragment::SetCall);

        connect(mResolver, &Resolver::SetRaise, game, &GameFragment::SetRaise);
        connect(mResolver, &Resolver::SetCheck, game, &GameFragment::SetCheck);
        connect(mResolver, &Resolver::ClearStatus, game, &GameFragment::ClearStatus);
        connect(mResolver, &Resolver::AvaliableActions, game, &GameFragment::AvaliableActions);
    }
    currentContainer->addWidget(newFragment);
    currentContainer->setCurrentWidget(newFragment);

}

BaseFragment* FragmentNavigator::Front() {
    return stack.front();
}

void FragmentNavigator::back() {
    qDebug("Navigator back");
    currentContainer->removeWidget(stack.back());
    delete stack.back();
    stack.pop_back();
    connectFragment(stack.back());
    stack.back()->onResume();
    currentContainer->setCurrentWidget(stack.back());
}



void FragmentNavigator::newRootScreen(QString tag) {
    qDebug("Navigator newRootScreen");
    BaseFragment *newFragment = this->screensFactory->create(tag);
    disconnectFragment(stack.back());
    stack.clear();
    connectFragment(newFragment);
    for(int i = currentContainer->count(); i >= 0; i--)
    {
        QWidget* widget = currentContainer->widget(i);
        currentContainer->removeWidget(widget);
        widget->deleteLater();
    }
    currentContainer->addWidget(newFragment);
    stack.push_back(newFragment);
}


BaseFragment* FragmentNavigator::getStartScreen() {
    return createAndConnect(this->screensFactory->createStart());
}


void FragmentNavigator::connectFragment(BaseFragment *fragment) {
    qDebug("Navigator connect slots");
    connect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    connect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    connect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    connect(fragment, &BaseFragment::Front, this, &FragmentNavigator::Front);
}

void FragmentNavigator::disconnectFragment(BaseFragment *fragment) {
    qDebug("Navigator disconnect slots");
    disconnect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    disconnect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    disconnect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    disconnect(fragment, &BaseFragment::Front, this, &FragmentNavigator::Front);
}

BaseFragment* FragmentNavigator::createAndConnect(QString tag) {
    BaseFragment *fragment = this->screensFactory->create(tag);
    qDebug("Navigator create screen");
    connectFragment(fragment);
    return fragment;
}
