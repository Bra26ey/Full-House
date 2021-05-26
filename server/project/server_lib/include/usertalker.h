#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>

#include "definitions.h"
#include "userbase.h"

#include "User.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

namespace network {

class UserTalker {
 public:
    UserTalker(std::shared_ptr<User> &user, Userbase &userbase, database::User &user_db);
    ~UserTalker() = default;
    void Start();
    bool IsUserWorks() const;

 public:
    boost::atomic<bool> is_remove;

 private:
    void OnHandleRequest();
    void HandleRequest();

    void SendAnswer();

    void HandleAutorisation();
    void HandleRegistration();
    void HandlePlayers();
    void JoinPlayer();
    void CreateGame();
    void HandlePing();
    void HandleError();
    void HandleAddMoney();
    void HandleMoneyInfo();
    void Disconnect();
    void Logout();

 private:
    io_context &context_;
    std::shared_ptr<User> user_;
    Userbase &userbase_;

    database::User &user_db_;
};

}  // namespace network
