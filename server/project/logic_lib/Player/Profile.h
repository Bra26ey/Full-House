//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <string>

namespace logic {

struct Profile {
    int id;
    std::string name;
    int avatar;
    int money;  // money is the entire player cash in client
};

}  // namespace logic
