#include "gametalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

static void Clean(std::istream &in) {
    char c = in.peek();
    while (c != EOF) {
        in >> c;
        c = in.peek();
    }
    in.clear();
}


namespace network {

uint64_t GameTalker::counter_ = 0;

void GameTalker::Start() {
    BOOST_LOG_TRIVIAL(info) << "GameRoom start work with connection";
    boost::asio::post(context_, boost::bind(&GameTalker::HandleGameProcess, this));
}

void GameTalker::HandleAdminRequest(std::shared_ptr<User> &user) {
    std::string str;
    user->in >> str;
    BOOST_LOG_TRIVIAL(info) << "GameRoom handleing admin request: " << str;
    Clean(user->in);
    if (str == "delete") {
        boost::asio::post(context_, boost::bind(&GameTalker::Delete, this));
        user->out << "{status: room will be deleted;}";
        async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
        return;
    } else if (str == "start") {
        is_gaming.store(true);
        user->out << "{status: game will be started;}";
        async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

void GameTalker::HandleUserRequest(std::shared_ptr<User> &user) {
    async_read_until(user->socket, user->read_buffer, "\n\r\n\r", boost::bind(&GameTalker::OnHandleUserRequest, this, user));
    // BOOST_LOG_TRIVIAL(info) << "GameRoom start work with connection";
}

void GameTalker::OnHandleUserRequest(std::shared_ptr<User> &user) {
    if (is_deleting_) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    std::string str;
    user->in >> str;
    if (str == "{game-command:" && is_gaming) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleGameRequest, this, user));
        return;
    }

    bool is_admin = (str == "{admin-command:" && user->name == users_.front()->name && !is_gaming);
    if (is_admin) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleAdminRequest, this, user));
        return;
    }

    if (str == "{leave:" && !is_gaming) {  // TODO(ANDY) add pos-ty to leave while gaming
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

int GameTalker::JoinPlayer(std::shared_ptr<User> &user) {
    if (is_gaming || is_remove || is_deleting_) {
        BOOST_LOG_TRIVIAL(info) << user->name << " not accepted to game. room-id: " << id;
        user->room_id = __UINT64_MAX__;
        user->is_talking.store(false);
        return -1;
    }

    users_mutex_.lock();
    users_.push_back(user);
    BOOST_LOG_TRIVIAL(info) << user->name << " accepted to game. room-id: " << id;
    ++online_users_;
    users_mutex_.unlock();
    user->is_gaming.store(true);
    user->is_talking.store(false);
    user->room_id = id;
    boost::asio::post(context_, boost::bind(&GameTalker::HandleUserRequest, this, user));
    return 0;
}

void GameTalker::HandleError(std::shared_ptr<User> &user) {
    Clean(user->in);
    user->out << "{error: unknown command;}";
    async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
    BOOST_LOG_TRIVIAL(info) << user->name << "'s request is unknown";
}

void GameTalker::Delete() {
    is_deleting_.store(true);
    boost::asio::post(context_, boost::bind(&GameTalker::OnDelete, this));
}

void GameTalker::OnDelete() {
    if (online_users_.load() != 0) {
        boost::asio::post(context_, boost::bind(&GameTalker::OnDelete, this));
        return;
    }

    BOOST_LOG_TRIVIAL(info) << "deleting room. room-id: " << id;
    is_remove.store(true);
}

void GameTalker::HandleLeaving(std::shared_ptr<User> &user) {
    Clean(user->in);

    BOOST_LOG_TRIVIAL(info) << user->name << " leave room";
    users_mutex_.lock();
    for (size_t i = 0; i < users_.size(); ++i) {
        if (users_[i]->name == user->name) {
            users_.erase(users_.begin() + i);
            break;
        }
    }
    users_mutex_.unlock();

    if (is_deleting_) {
        user->out << "{room is closed}";
    } else {
        user->out << "{you leave this room}";
    }

    write(user->socket, user->write_buffer);  // ATTENTION!

    user->is_gaming.store(false);
    user->room_id = __UINT64_MAX__;

    --online_users_;
}

void GameTalker::HandleGameRequest(std::shared_ptr<User> &user) {
    Clean(user->in);
    BOOST_LOG_TRIVIAL(info) << user->name << " send game-request";
    user->out << "{status: game-request is handled;}";
    // DO STH IMPORTANT
    async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
}

void GameTalker::HandleGameProcess() {
    // reenter(this) {
    //     while (true) {
    //         
    //     }
    // }
}

}  // namespace network
