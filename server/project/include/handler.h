#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "user.h"

namespace network {

typedef std::queue<std::shared_ptr<User>> user_queue;

class Handler {
 public:
    Handler(io_context &context, Userbase &userbase) : context_(context),
                                                       autorised_(userbase.autorised),
                                                       creating_game_(userbase.creating_game),
                                                       accepting_game_(userbase.accepting_game) {}
    Handler(Handler &other) = delete;
    ~Handler();

 private:
    io_context &context_;

    user_queue &autorised_;
    user_queue &creating_game_;
    user_queue &accepting_game_;
};

}  // namespace network
