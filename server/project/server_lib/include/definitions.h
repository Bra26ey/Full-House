#pragma once

#include <memory>
#include <boost/log/trivial.hpp>

#include "tsqueue.h"
#include "net-user.h"

namespace network {

typedef TSQueue<std::shared_ptr<User>> user_queue;

class UserTalker;
class GameTalker;

typedef std::vector<std::shared_ptr<UserTalker>> usertalk_vector;
typedef std::vector<std::shared_ptr<GameTalker>> gametalk_vector;

}
