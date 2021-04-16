#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "user.h"

class HandleProcess;

namespace network {

class GameRoom {
 public:
    explicit GameRoom(io_context &context) : context_(context) {}
    GameRoom(GameRoom  &other) = delete;
    ~GameRoom();

    void Step();
    void AddUser(std::shared_ptr<User> &user);

 private:
    io_context &context_;
    std::vector<User> _users;
};

}  // namespace network
