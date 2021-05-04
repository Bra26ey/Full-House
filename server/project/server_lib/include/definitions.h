#pragma once

#include <memory>
#include <boost/log/trivial.hpp>

#include "tsqueue.h"
#include "user.h"

namespace network {

typedef TSQueue<std::shared_ptr<User>> user_queue;

}
