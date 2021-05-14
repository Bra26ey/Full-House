#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/asio/coroutine.hpp>

#include "definitions.h"
#include "userbase.h"

#include "User.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

namespace network {

class UserTalker : public boost::asio::coroutine {
 public:
    UserTalker(std::shared_ptr<User> &user, Userbase &userbase, database::User &user_database);
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
    void HandlePing();
    void HandleError();
    void Disconnect();
    void Logout();

 private:
    io_context &context_;
    std::shared_ptr<User> user_;
    Userbase &userbase_;

    database::User &user_database_;
};

}  // namespace network
