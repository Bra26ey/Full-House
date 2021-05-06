#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/asio/coroutine.hpp>

#include "definitions.h"
#include "userbase.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

namespace network {

class UserTalker : public boost::asio::coroutine {
 public:
    UserTalker(std::shared_ptr<User> user, Userbase &userbase) : is_remove(false),
                                                                 context_(user->context),
                                                                 user_(user),
                                                                 userbase_(userbase) {};
    ~UserTalker() = default;
    void Start();
    bool IsUserWorks() const;

 public:
    boost::atomic<bool> is_remove;

 private:
    void HandleAutorisation();
    void HandlePlayers();
    void JoinPlayer();
    void CreateGame();
    void HandleRequest();
    void HandleError();
    void Disconnect();
    void Logout();

 private:
    io_context &context_;
    std::shared_ptr<User> user_;
    Userbase &userbase_;
};

}  // namespace network
