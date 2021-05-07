#include "server.h"

#include <vector>

#include <boost/log/trivial.hpp>

#include "msgmaker.h"

namespace network {

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
        if (usertalkers_[i]->is_remove) {
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
    for (size_t i = 0; i < gametalkers_.size(); ++i) {
        if (gametalkers_[i]->is_remove) {
            gametalkers_.erase(gametalkers_.begin() + i);
            BOOST_LOG_TRIVIAL(info) << "deleted room is removed";
        }
    }
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
    acceptor_.accept(user->socket);
    BOOST_LOG_TRIVIAL(info) << "user accepted";
    auto usertalker = std::make_shared<UserTalker>(user, userbase_);
    usertalkers_mutex_.lock();
    usertalkers_.push_back(usertalker);
    usertalkers_mutex_.unlock();
    auto new_user = std::make_shared<User>(context_);
    context_.post(boost::bind(&Server::HandleAcception, this, new_user));
}

void Server::CreateRooms() {
    if (userbase_.creating_game.IsEmpty()) {
        context_.post(boost::bind(&Server::CreateRooms, this));
        return;
    }

    auto user = userbase_.creating_game.Pop();
    auto gametalker = std::make_shared<GameTalker>(context_);

    int code = gametalker->JoinPlayer(user);
    if (code != 0) {
        context_.post(boost::bind(&Server::CreateRooms, this));
        read_until(user->socket, user->read_buffer, "/n/r/n/r");
        user->out << MsgServer::CreateRoomFailed();
        write(user->socket, user->write_buffer);
        return;
    }

    gametalkers_mutex_.lock();
    gametalker->Start();
    gametalkers_.push_back(gametalker);
    gametalkers_mutex_.unlock();
    context_.post(boost::bind(&Server::CreateRooms, this));
}

void Server::JoinPlayers() {
    if (userbase_.accepting_game.IsEmpty()) {
        context_.post(boost::bind(&Server::JoinPlayers, this));
        return;
    }

    auto user = userbase_.accepting_game.Pop();
    gametalkers_mutex_.lock();
    bool flag = true;
    for (auto &it : gametalkers_) {
        if (it->id == user->room_id) {
            it->JoinPlayer(user);
            flag = false;
            break;
        }
    }

    if (flag) {
        BOOST_LOG_TRIVIAL(info) << user->name << " not accepted to game. can't find room-id: " << user->room_id;
        read_until(user->socket, user->read_buffer, "/n/r/n/r");
        user->out << MsgServer::JoinRoomFaild(user->room_id);
        write(user->socket, user->write_buffer);
        user->room_id = __UINT64_MAX__;
        user->is_talking.store(false);
    }

    gametalkers_mutex_.unlock();
    context_.post(boost::bind(&Server::JoinPlayers, this));
}

}