#pragma once

#include "definitions.h"

namespace network {

struct Userbase {
    user_queue waitng_autorisation;
    user_queue autorised;
    user_queue creating_game;
    user_queue accepting_game;
};

}  // namespace network
