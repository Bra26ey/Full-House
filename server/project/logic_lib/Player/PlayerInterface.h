//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_PLAYERINTERFACE_H
#define FULL_HOUSE_PLAYERINTERFACE_H

struct PlayerInterface {
    virtual void Fold() = 0;
    virtual int Call(int to_call) = 0;
    virtual void Check() = 0;
    virtual int Raise(int raised_money) = 0;
};

#endif //FULL_HOUSE_PLAYERINTERFACE_H
