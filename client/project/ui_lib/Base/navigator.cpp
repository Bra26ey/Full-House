#include "navigator.h"
#include "screenfactory.h"

using namespace screens;

FragmentNavigator::FragmentNavigator(
        QStackedWidget *container,
        BaseScreensFactory *screensFactory,
        Resolver* resolver) : mResolver(resolver) {

    this->screensFactory = screensFactory;
    this->currentContainer = container;
    BaseFragment* startFragment = getStartScreen();
    connectFragment(resolver);

    auto tmp = static_cast<LoginFragment*>(startFragment);
    connect(mResolver, &Resolver::WrongDataAutorisation, tmp, &LoginFragment::WrongData, Qt::QueuedConnection);

    this->stack.push_back(startFragment);
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
        connect(mResolver, &Resolver::DeletePlayer, game, &GameFragment::DeletePlayer, Qt::QueuedConnection);
        connect(mResolver, &Resolver::DrawPlayer, game, &GameFragment::DrawPlayer, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetMinBet, game, &GameFragment::SetMinBet, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetMaxBet, game, &GameFragment::SetMaxBet, Qt::QueuedConnection);

        connect(mResolver, &Resolver::EndGame, game, &GameFragment::EndGame, Qt::QueuedConnection);
        connect(mResolver, &Resolver::FlipTableCards, game, &GameFragment::FlipTableCards, Qt::QueuedConnection);
        connect(mResolver, &Resolver::DeleteAllCardsFromTable, game, &GameFragment::DeleteAllCardsFromTable, Qt::QueuedConnection);

        connect(mResolver, &Resolver::AddCardToTable, game, &GameFragment::AddCardToTable, Qt::QueuedConnection);
        connect(mResolver, &Resolver::FlipAllCards, game, &GameFragment::FlipAllCards, Qt::QueuedConnection);


        connect(mResolver, &Resolver::ShowActions, game, &GameFragment::ShowActions, Qt::QueuedConnection);
        connect(mResolver, &Resolver::BlockActions, game, &GameFragment::BlockActions, Qt::QueuedConnection);
        connect(mResolver, &Resolver::UnBlockActions, game, &GameFragment::UnBlockActions, Qt::QueuedConnection);
        connect(mResolver, &Resolver::ShowStart, game, &GameFragment::ShowStart, Qt::QueuedConnection);

        connect(mResolver, &Resolver::MakeDealer, game, &GameFragment::MakeDealer, Qt::QueuedConnection);
        connect(mResolver, &Resolver::DisplayWinner, game, &GameFragment::DisplayWinner, Qt::QueuedConnection);
        connect(mResolver, &Resolver::CurrentTurn, game, &GameFragment::CurrentTurn, Qt::QueuedConnection);
        connect(mResolver, &Resolver::GiveCards, game, &GameFragment::GiveCards, Qt::QueuedConnection);

        connect(mResolver, &Resolver::FlipCards, game, &GameFragment::FlipCards, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetBet, game, &GameFragment::SetBet, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetFold, game, &GameFragment::SetFold, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetCall, game, &GameFragment::SetCall, Qt::QueuedConnection);

        connect(mResolver, &Resolver::SetRaise, game, &GameFragment::SetRaise, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetCheck, game, &GameFragment::SetCheck, Qt::QueuedConnection);
        connect(mResolver, &Resolver::ClearStatus, game, &GameFragment::ClearStatus, Qt::QueuedConnection);
        connect(mResolver, &Resolver::AvaliableActions, game, &GameFragment::AvaliableActions, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetMoneyInBank, game, &GameFragment::SetMoneyInBank, Qt::QueuedConnection);
        connect(mResolver, &Resolver::DeleteWinnerDisplay, game, &GameFragment::DeleteWinnerDisplay, Qt::QueuedConnection);
        connect(mResolver, &Resolver::ClearBank, game, &GameFragment::ClearBank, Qt::QueuedConnection);
        connect(mResolver, &Resolver::DeleteAllPlayersCards, game, &GameFragment::DeleteAllPlayersCards, Qt::QueuedConnection);
        connect(mResolver, &Resolver::SetMoney, game, &GameFragment::SetMoney, Qt::QueuedConnection);
    } else if (tag == REGISTRATION_TAG) {
        RegistrationFragment* reg = static_cast<RegistrationFragment*>(newFragment);
        connect(mResolver, &Resolver::WrongDataRegistration, reg, &RegistrationFragment::WrongDataRegistration, Qt::QueuedConnection);
        connect(mResolver, &Resolver::RightDataRegistration, reg, &RegistrationFragment::RightDataRegistration, Qt::QueuedConnection);
    } else if (tag == SEARCH_TAG) {
        GameSearchFragment* src = static_cast<GameSearchFragment*>(newFragment);
        connect(mResolver, &Resolver::WrongDataRoomJoin, src, &GameSearchFragment::WrongDataRoomJoin, Qt::QueuedConnection);
    }
    currentContainer->addWidget(newFragment);
    currentContainer->setCurrentWidget(newFragment);

}

BaseFragment* FragmentNavigator::Front() {
    return stack.front();
}

void FragmentNavigator::back() {
    currentContainer->removeWidget(stack.back());
    delete stack.back();
    stack.pop_back();
    connectFragment(stack.back());
    stack.back()->onResume();
    currentContainer->setCurrentWidget(stack.back());
}



void FragmentNavigator::newRootScreen(QString tag) {
    BaseFragment *newFragment = this->screensFactory->create(tag);
    disconnectFragment(stack.back());
    stack.clear();
    connectFragment(newFragment);
    for(int i = currentContainer->count(); i >= 0; i--) {
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
    connect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    connect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    connect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    connect(fragment, &BaseFragment::Front, this, &FragmentNavigator::Front);
}

void FragmentNavigator::disconnectFragment(BaseFragment *fragment) {
    disconnect(fragment, &BaseFragment::back, this, &FragmentNavigator::back);
    disconnect(fragment, &BaseFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    disconnect(fragment, &BaseFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    disconnect(fragment, &BaseFragment::Front, this, &FragmentNavigator::Front);
}

BaseFragment* FragmentNavigator::createAndConnect(QString tag) {
    BaseFragment *fragment = this->screensFactory->create(tag);
    connectFragment(fragment);
    return fragment;
}
