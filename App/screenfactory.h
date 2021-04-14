#ifndef SCREENFACTORY_H
#define SCREENFACTORY_H

#include "basescreensfactory.h"

namespace screens {
    static const QString LOGIN_TAG = "login";
    static const QString REGISTRATION_TAG = "registration";
    static const QString MAIN_TAG = "main";
}

class ScreensFactory : public BaseScreensFactory {
public:
    ScreensFactory();
    ~ScreensFactory();

    BaseFragment* create(QString tag) override;
    QString createStart() override;
};

#endif // SCREENFACTORY_H
