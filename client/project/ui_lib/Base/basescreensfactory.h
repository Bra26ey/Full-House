#ifndef BASESCREENSFACTORY_H
#define BASESCREENSFACTORY_H

#include "basefragment.h"

#include <QString>


class BaseScreensFactory {
public:
    BaseScreensFactory();
    virtual ~BaseScreensFactory();

    virtual BaseFragment* create(QString tag) = 0;
    virtual QString createStart() = 0;
};

#endif // BASESCREENSFACTORY_H
