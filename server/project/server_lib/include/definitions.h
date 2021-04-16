#pragma once

#include <queue>

#include "tsqueue.h"
#include "user.h"

namespace network {
    typedef TSQueue<std::shared_ptr<User>> user_queue;
}