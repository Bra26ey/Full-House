#ifndef SCREENFACTORY_H
#define SCREENFACTORY_H

#include "basescreensfactory.h"

#include "loginfragment.h"
#include "registrationfragment.h"
#include "menufragment.h"
#include "settingsfragment.h"
#include "gamesearchfragment.h"
#include "gamefragment.h"
#include "rulesfragment.h"
#include "creditmenufragment.h"
#include "equationsfragment.h"


namespace screens {
    static const QString LOGIN_TAG = "login";
    static const QString REGISTRATION_TAG = "registration";
    static const QString MAIN_TAG = "main";
    static const QString SETTINGS_TAG = "settings";
    static const QString SEARCH_TAG = "search";
    static const QString GAME_TAG = "game";
    static const QString RULE_TAG = "rule";
    static const QString CREDIT_TAG = "credit";
    static const QString EQUATION_TAG = "equation";
}

class ScreensFactory : public BaseScreensFactory {
public:
    ScreensFactory();
    ~ScreensFactory();

    BaseFragment* create(QString tag) override;
    QString createStart() override;
};

#endif // SCREENFACTORY_H
