#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "user.h"

namespace network {

typedef std::queue<std::shared_ptr<User>> user_queue;

class Handler {
 public:
    Handler(io_context &context, user_queue &autorised_, user_queue &creating_game_, user_queue &accepting_game_);
    Handler(Handler &other) = delete;
    ~Handler();

 private:
    io_context &context_;

    user_queue &autorised_;
    user_queue &creating_game_;
    user_queue &accepting_game_;
};

}  // namespace network
