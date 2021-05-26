#include "screenfactory.h"


using namespace screens;

ScreensFactory::ScreensFactory() {

}

ScreensFactory::~ScreensFactory() {

}

BaseFragment* ScreensFactory::create(QString tag) {
    qDebug("ScreensFactory create");

    if (tag == LOGIN_TAG) {
        return new LoginFragment;
    }
    if (tag == REGISTRATION_TAG) {
        return new RegistrationFragment;
    }
    if (tag == MAIN_TAG) {
        return new MenuFragment;
    }
    if (tag == SETTINGS_TAG) {
        return new SettingsFragment;
    }
    if (tag == SEARCH_TAG) {
        return new GameSearchFragment;
    }
    if (tag == GAME_TAG) {
        return new GameFragment;
    }
    if (tag == RULE_TAG) {
        return new RulesFragment;
    }
    if (tag == CREDIT_TAG) {
        return new CreditMenuFragment;
    }
    if (tag == EQUATION_TAG) {
        return new EquationsFragment;
    }
    return nullptr;
}

QString ScreensFactory::createStart() {
    return LOGIN_TAG;
}
