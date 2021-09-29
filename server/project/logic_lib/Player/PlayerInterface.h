//
// Created by aleksandr on 15.04.2021.
//

#pragma once

namespace logic {

struct PlayerInterface {
    virtual void Fold() = 0;
    virtual int Call(int to_call) = 0;
    virtual void Check() = 0;
    virtual int Raise(int raised_money, int reraise) = 0;
};

}  // namespace logic
