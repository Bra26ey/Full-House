#pragma once

#include <queue>

#include "user.h"

namespace network {
    typedef std::queue<std::shared_ptr<User>> user_queue;
}