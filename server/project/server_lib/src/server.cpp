#include "server.h"

#include <vector>
#include <algorithm>

#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "msgmaker.h"

namespace bs = boost::system;

namespace network {

Server::Server() : context_(),
                   endpoint_(address::from_string(std::string(SERVER_IP)), PORT),
                   acceptor_(context_) {}

Server::~Server() {}

void Server::Start() {
    context_.post(boost::bind(&Server::StartAccepting, this));
    context_.post(boost::bind(&Server::CleanUserTalkers, this));
    context_.post(boost::bind(&Server::CleanGameTalkers, this));
    context_.post(boost::bind(&Server::CreateRooms, this));
    context_.post(boost::bind(&Server::JoinPlayers, this));

    const size_t thread_nom = boost::thread::hardware_concurrency();
    BOOST_LOG_TRIVIAL(info) << "threads avaiable: " << thread_nom;
    StartListen(thread_nom);
    threads_.join_all();
}

void Server::ListenThread() {
    context_.run();
}

void Server::StartListen(size_t thread_count) {
    BOOST_LOG_TRIVIAL(info) << "start server";
    for (size_t i = 0; i < thread_count; ++i) {
        threads_.create_thread(boost::bind(&Server::ListenThread, this));
    }
}

void Server::CleanUserTalkers() {
    usertalkers_mutex_.lock();
    for (size_t i = 0; i < usertalkers_.size(); ++i) {
        if (usertalkers_[i]->is_remove.load()) {
            usertalkers_.erase(usertalkers_.begin() + i);
            BOOST_LOG_TRIVIAL(info) << "disconnected user is removed";
        } else if (!usertalkers_[i]->IsUserWorks()) {
            BOOST_LOG_TRIVIAL(info) << "user start work in pos " << i;
            usertalkers_[i]->Start();
        }
    }
    usertalkers_mutex_.unlock();
    context_.post(boost::bind(&Server::CleanUserTalkers, this));
}

void Server::CleanGameTalkers() {
    gametalkers_mutex_.lock();

    // auto it = std::find_if(gametalkers_.begin(), gametalkers_.end(),
    //                        [](const std::shared_ptr<GameTalker> &current) { return current->is_remove.load(); });

    // if (it != gametalkers_.end()) {
    //     gametalkers_.erase(it);
    // }

    std::erase_if(gametalkers_, [](const std::shared_ptr<GameTalker> &current) { return current->is_remove.load(); });
    // uncomment later. It works, but vscode image an error

    gametalkers_mutex_.unlock();
    context_.post(boost::bind(&Server::CleanGameTalkers, this));
}

void Server::StartAccepting() {
    BOOST_LOG_TRIVIAL(info) << "start acceptor on ep = " << endpoint_;
    acceptor_.open(endpoint_.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint_);
    acceptor_.listen();
    auto user = std::make_shared<User>(context_);
    HandleAcception(user);
}

void Server::HandleAcception(std::shared_ptr<User> &user) {
    BOOST_LOG_TRIVIAL(info) << "waiting for acception";
    acceptor_.async_accept(user->socket, [this, user](bs::error_code error) {
        if (!error) {
            AcceptionDone(user);
        } else {
            AcceptionFailed();
        }
    });
}

void Server::AcceptionDone(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << "user accepted";
    auto usertalker = std::make_shared<UserTalker>(user, userbase_, user_db_);
    BOOST_LOG_TRIVIAL(info) << "make usertalker";
    usertalkers_mutex_.lock();
    BOOST_LOG_TRIVIAL(info) << "before push to queue";
    usertalkers_.push_back(usertalker);
    BOOST_LOG_TRIVIAL(info) << "push to queue";
    usertalkers_mutex_.unlock();
    BOOST_LOG_TRIVIAL(info) << "make new user";
    auto new_user = std::make_shared<User>(context_);
    BOOST_LOG_TRIVIAL(info) << "before new accepting";
    context_.post(boost::bind(&Server::HandleAcception, this, new_user));
}

void Server::AcceptionFailed() {
    BOOST_LOG_TRIVIAL(info) << "user not accepted";
    auto new_user = std::make_shared<User>(context_);
    context_.post(boost::bind(&Server::HandleAcception, this, new_user));
}

void Server::CreateRooms() {
    if (userbase_.creating_game.IsEmpty()) {
        context_.post(boost::bind(&Server::CreateRooms, this));
        return;
    }

    auto user = userbase_.creating_game.Pop();
    BOOST_LOG_TRIVIAL(info) << user->name << " trying to create game";
    auto gametalker = std::make_shared<GameTalker>(context_, board_db_, user_db_, user);

    gametalkers_mutex_.lock();
    gametalkers_.push_back(gametalker);
    gametalkers_mutex_.unlock();
    context_.post(boost::bind(&Server::CreateRooms, this));
}

void Server::JoinPlayers() {
    if (userbase_.accepting_game.IsEmpty()) {
        context_.post(boost::bind(&Server::JoinPlayers, this));
        return;
    }

    const std::lock_guard<std::mutex> lock(gametalkers_mutex_);

    auto user = userbase_.accepting_game.Pop();

    BOOST_LOG_TRIVIAL(info) << user->name << " trying accepted to game with room-id: " << user->room_id;

    auto it = std::find_if(gametalkers_.begin(), gametalkers_.end(),
                          [user](const std::shared_ptr<GameTalker> &current) { return current->id == user->room_id; });

    if (it == gametalkers_.end()) {  // must be reworked!
        BOOST_LOG_TRIVIAL(info) << user->name << " not accepted to game. can't find room-id: " << user->room_id;
        read_until(user->socket, user->read_buffer, std::string(MSG_END));
        boost::property_tree::read_json(user->in, user->last_msg);
        user->out << MsgServer::JoinRoomFailed(user->room_id);
        write(user->socket, user->write_buffer);
        user->room_id = __UINT64_MAX__;
        user->is_talking.store(false);
        context_.post(boost::bind(&Server::JoinPlayers, this));
        return;
    }

    it->get()->JoinPlayer(user);

    context_.post(boost::bind(&Server::JoinPlayers, this));
}

}  // namespace network
