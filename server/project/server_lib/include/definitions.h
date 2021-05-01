#pragma once

#include <memory>

#include "tsqueue.h"
#include "user.h"

namespace network {
    typedef TSQueue<std::shared_ptr<User>> user_queue;
    // typedef TSQueue<int> user_queue;
}
