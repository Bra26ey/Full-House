#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/asio/coroutine.hpp>

#include "definitions.h"
#include "tablepositions.h"

#include "ConfigurationHandler.h"
#include "HandProcess.h"

#include "Board.h"

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class GameTalker : public boost::asio::coroutine {
 public:
    GameTalker(io_context &context, database::Board &board_db, database::User &user_db, std::shared_ptr<User> &user);
    ~GameTalker();
    int JoinPlayer(std::shared_ptr<User> &user);

 public:
    uint64_t id;
    boost::atomic<bool> is_remove;
    boost::atomic<bool> is_gaming;

 private:
    void OnHandleUserRequest(std::shared_ptr<User> user);
    void HandleUserRequest(std::shared_ptr<User> user);

    void SendAnswer(std::shared_ptr<User> user);

    void JoinPlayerFailed(std::shared_ptr<User> user);
    void CreatingFailed(std::shared_ptr<User> user);

    void HandleAdminRequest(std::shared_ptr<User> user);
    void HandleGameRequest(std::shared_ptr<User> user);
    void HandleLeaving(std::shared_ptr<User> user);
    void HandleError(std::shared_ptr<User> user);
    void HandleGameStatus(std::shared_ptr<User> user);

    void Delete();

    void Start();
    void HandleGameProcess();

    void UpdateTableDatabase();
    void InitReservedMoney();

 private:
    io_context &context_;

    database::Board &board_db_;
    database::User &user_db_;

    logic::HandProcess handprocess_;

    std::vector<std::shared_ptr<User>> users_;
    std::mutex users_mutex_;

    boost::atomic<bool> is_deleting_;
    boost::atomic<uint64_t> admin_id_;

    TablePositions positions_;
};

}  // namespace network
