#include "gametalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "msgmaker.h"

using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;

namespace network {

uint64_t GameTalker::counter_ = 0;

void GameTalker::Start() {
    BOOST_LOG_TRIVIAL(info) << "GameRoom Start Game";
    boost::asio::post(context_, boost::bind(&GameTalker::HandleGameProcess, this));
}

void GameTalker::HandleAdminRequest(std::shared_ptr<User> &user) {
    BOOST_LOG_TRIVIAL(info) << "GameRoom handleing admin request: ";

    std::string command = user->last_msg.get<std::string>("command");

    if (command == "start") {
        if (online_users_.load() > 1) {
            is_gaming.store(true);
            user->out << MsgServer::StartGameDone();
        } else {
            user->out << MsgServer::StartGameFailed();
        }
        async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
        boost::asio::post(context_, boost::bind(&GameTalker::Start, this));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

void GameTalker::HandleUserRequest(std::shared_ptr<User> &user) {
    async_read_until(user->socket, user->read_buffer, "\n\r\n\r", boost::bind(&GameTalker::OnHandleUserRequest, this, user));
}

void GameTalker::OnHandleUserRequest(std::shared_ptr<User> &user) {
    if (is_remove) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    pt::read_json(user->in, user->last_msg);
    std::string command_type = user->last_msg.get<std::string>("command-type");

    if (command_type == "game-command" && is_gaming) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleGameRequest, this, user));
        return;
    }

    bool is_admin = (command_type == "room-admin" && user->name == users_.front()->name && !is_gaming);
    if (is_admin) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleAdminRequest, this, user));
        return;
    }

    if (command_type == "room-basic" && !is_gaming) {  // TODO(ANDY) add pos-ty to leave while gaming
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

int GameTalker::JoinPlayer(std::shared_ptr<User> &user) {
    if (is_gaming || is_remove) {
        BOOST_LOG_TRIVIAL(info) << user->name << " not accepted to game. room-id: " << id;
        user->room_id = __UINT64_MAX__;
        read_until(user->socket, user->read_buffer, "\n\r\n\r");
        pt::read_json(user->in, user->last_msg);
        user->out << MsgServer::JoinRoomFaild(id);
        write(user->socket, user->write_buffer);
        user->is_talking.store(false);
        return -1;
    }

    const pt::ptree &parametrs = user->last_msg.get_child("parametrs");
    auto password = parametrs.get<std::string>("password");

    BOOST_LOG_TRIVIAL(info) << user->name << " accepted to game. room-id: " << id;

    ++online_users_;
    users_mutex_.lock();
    users_.push_back(user);
    bool is_admin = (users_.size() == 1);
    users_mutex_.unlock();

    user->is_gaming.store(true);
    user->is_talking.store(false);
    user->room_id = id;

    read_until(user->socket, user->read_buffer, "\n\r\n\r");
    pt::read_json(user->in, user->last_msg);

    if (is_admin) {
        user->out << MsgServer::CreateRoomDone(id, password);
    } else {
        user->out << MsgServer::JoinRoomDone(id);
    }
    write(user->socket, user->write_buffer);

    boost::asio::post(context_, boost::bind(&GameTalker::HandleUserRequest, this, user));
    return 0;
}

void GameTalker::HandleError(std::shared_ptr<User> &user) {
    user->out << MsgServer::Error();
    async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
    BOOST_LOG_TRIVIAL(info) << user->name << "'s request is unknown";
}

void GameTalker::Delete() {
    is_remove.store(true);
    BOOST_LOG_TRIVIAL(info) << "deleting room. room-id: " << id;
}

void GameTalker::HandleLeaving(std::shared_ptr<User> &user) {
    BOOST_LOG_TRIVIAL(info) << user->name << " leave room";
    users_mutex_.lock();
    for (size_t i = 0; i < users_.size(); ++i) {
        if (users_[i]->name == user->name) {
            users_.erase(users_.begin() + i);
            break;
        }
    }
    users_mutex_.unlock();

    user->out << MsgServer::LeaveRoomDone();

    write(user->socket, user->write_buffer);  // ATTENTION!

    user->is_gaming.store(false);
    user->room_id = __UINT64_MAX__;

    --online_users_;

    if (online_users_ == 0) {
        boost::asio::post(context_, boost::bind(&GameTalker::Delete, this));
    }
}

void GameTalker::HandleGameRequest(std::shared_ptr<User> &user) {
    BOOST_LOG_TRIVIAL(info) << user->name << " send game-request";
    user->out << "{status: game-request is handled;}\n\r\n\r";
    // DO STH IMPORTANT
    async_write(user->socket, user->write_buffer, boost::bind(&GameTalker::HandleUserRequest, this, user));
}

void GameTalker::HandleGameProcess() {
    reenter(this) {
        while (true) {
            yield {
                // handprocess_.INICIALIZE()
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.ShuffleCards();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.DealCards();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.Preflop();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.Flop();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.Turn();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.River();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                // handprocess_.PotDistribution
            }
        }
    }
}

}  // namespace network