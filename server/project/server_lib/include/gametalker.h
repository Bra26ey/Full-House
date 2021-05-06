#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/asio/coroutine.hpp>

#include "definitions.h"
#include "gameroom.h"

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class GameTalker : public boost::asio::coroutine {
 public:
    GameTalker(io_context &context) : is_remove(false),
                                      is_gaming(false),
                                      context_(context),
                                      is_deleting_(false) {
       id = counter_++;
    };
    ~GameTalker() = default;
    void Start();
    int JoinPlayer(std::shared_ptr<User> &user);

 public:
    uint64_t id;
    boost::atomic<bool> is_remove;
    boost::atomic<bool> is_gaming;

 private:
    void HandleUserRequest(std::shared_ptr<User> &user);
    void OnHandleUserRequest(std::shared_ptr<User> &user);

    void HandleAdminRequest(std::shared_ptr<User> &user);
    void HandleGameRequest(std::shared_ptr<User> &user);
    void HandleLeaving(std::shared_ptr<User> &user);
    void HandleError(std::shared_ptr<User> &user);

    void Delete();
    void OnDelete();

    void HandleGameProcess();

 private:
    io_context &context_;

    std::vector<std::shared_ptr<User>> users_;
    std::mutex users_mutex_;
    

    boost::atomic<bool> is_deleting_;
    boost::atomic<uint64_t> online_users_;

    static uint64_t counter_;
};

}  // namespace network
