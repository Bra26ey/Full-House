//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_PLAYERINTERFACE_H
#define FULL_HOUSE_PLAYERINTERFACE_H

struct PlayerInterface {
    virtual void Fold() = 0;
    virtual void Call() = 0;
    virtual void Check() = 0;
    virtual void Raise() = 0;
};

#endif //FULL_HOUSE_PLAYERINTERFACE_H
