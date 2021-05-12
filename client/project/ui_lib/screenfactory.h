#ifndef SCREENFACTORY_H
#define SCREENFACTORY_H

#include "basescreensfactory.h"

namespace screens {
    static const QString LOGIN_TAG = "login";
    static const QString REGISTRATION_TAG = "registration";
    static const QString MAIN_TAG = "main";
    static const QString SETTINGS_TAG = "settings";
    static const QString SEARCH_TAG = "search";
    static const QString GAME_TAG = "game";
    static const QString RULE_TAG = "rule";
}

class ScreensFactory : public BaseScreensFactory {
public:
    ScreensFactory();
    ~ScreensFactory();

    BaseFragment* create(QString tag) override;
    QString createStart() override;
};

#endif // SCREENFACTORY_H
