//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_PROFILE_H
#define FULL_HOUSE_PROFILE_H

#include <string>

struct Profile {
    int id;
    std::string name;
    int avatar;
    int money;  // money is the entire player cash in client
};

#endif //FULL_HOUSE_PROFILE_H
