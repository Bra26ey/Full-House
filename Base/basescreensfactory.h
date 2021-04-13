#ifndef BASESCREENSFACTORY_H
#define BASESCREENSFACTORY_H

#include "basefragment.h"

#include <QString>


class BaseScreensFactory {
public:
    BaseScreensFactory();
    virtual ~BaseScreensFactory();

    virtual BaseFragment* create(QString tag);
    virtual QString createStart();
};

#endif // BASESCREENSFACTORY_H
