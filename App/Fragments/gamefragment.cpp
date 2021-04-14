#include "gamefragment.h"

GameFragment::GameFragment() {

}

GameFragment::~GameFragment() {
    delete Player;
    delete[] OtherPlayers;
    delete PlayTable;

    delete BetButton;
    delete RaiseButton;
    delete FoldButton;
    delete CheckButton;

    delete LeaveButton;
    delete SettingsButton;
}


void GameFragment::onBetPressed() {

}

void GameFragment::onCheckPressed() {

}

void GameFragment::onRaisePressed() {

}

void GameFragment::onFoldPressed() {

}

void GameFragment::onLeavePressed() {

}

void GameFragment::onSettingsPressed() {

}

void GameFragment::ChangedSlider() {

}
