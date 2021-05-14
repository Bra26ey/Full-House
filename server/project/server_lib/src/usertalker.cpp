#include "usertalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "msgmaker.h"

using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;

namespace network {

UserTalker::UserTalker(std::shared_ptr<User> &user, Userbase &userbase, database::User &user_database)
           : is_remove(false),
             context_(user->context),
             user_(user),
             userbase_(userbase),
             user_database_(user_database) {}

void UserTalker::Start() {
    user_->is_talking.store(true);
    BOOST_LOG_TRIVIAL(info) << "UserTalker start work with connection";
    boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
}

bool UserTalker::IsUserWorks() const {
    return (user_->is_talking.load() || user_->is_gaming.load());
}

void UserTalker::HandleAutorisation() {
    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    auto name = parametrs.get<std::string>("login");
    auto password = parametrs.get<std::string>("password");

    user_->is_autorised = true;

    // auto answer = user_database_.AuthUser(name, password);
    // user_->is_autorised = answer.second == database::OK;

    if (user_->is_autorised) {
        user_->name = name;
        user_->out << MsgServer::AutorisationDone();
        BOOST_LOG_TRIVIAL(info) << "user is autorised. name: " << user_->name;
    } else {
        user_->out << MsgServer::AutorisationFaild();
        BOOST_LOG_TRIVIAL(info) << "autorisation: invalid data";
    }
    async_write(user_->socket, user_->write_buffer, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::CreateGame() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to create room";
    user_->out << MsgServer::CreateRoomOn();
    async_write(user_->socket, user_->write_buffer, boost::bind(&user_queue::Push, &userbase_.creating_game, user_));
}


void UserTalker::JoinPlayer() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to accept game";

    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    user_->room_id = parametrs.get<uint64_t>("id");

    user_->out << MsgServer::JoinRoomOn(user_->room_id);
    async_write(user_->socket, user_->write_buffer, boost::bind(&user_queue::Push, &userbase_.accepting_game, user_));
}

void UserTalker::HandleError() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "'s request is unknown";

    user_->out << MsgServer::Error();
    async_write(user_->socket, user_->write_buffer, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::HandlePing() {
    user_->out << MsgServer::Ping();
    async_write(user_->socket, user_->write_buffer, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::Disconnect() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "is disconnected";

    user_->out << MsgServer::Disconnect();
    write(user_->socket, user_->write_buffer);
    is_remove.store(true);
}

void UserTalker::Logout() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "is loged out";

    user_->name.clear();
    user_->is_autorised = false;

    user_->out << MsgServer::Logout();
    async_write(user_->socket, user_->write_buffer, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::HandleRequest() {
    reenter(this) {
        while (true) {
            yield async_read_until(user_->socket, user_->read_buffer, "\n\r\n\r", boost::bind(&UserTalker::HandleRequest, this));
            yield {
                pt::read_json(user_->in, user_->last_msg);
                BOOST_LOG_TRIVIAL(info) << "handle user request: ";

                std::string command_type = user_->last_msg.get<std::string>("command-type");

                if (command_type == "ping") {
                    boost::asio::post(context_, boost::bind(&UserTalker::HandlePing, this));
                    return;
                }

                if (command_type != "basic") {
                    boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
                    return;
                }

                std::string command = user_->last_msg.get<std::string>("command");

                if (!user_->is_autorised) {
                    if (command == "autorisation") {
                        boost::asio::post(context_, boost::bind(&UserTalker::HandleAutorisation, this));
                    } else {
                        boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
                    }
                    return;
                }

                if (command == "create-room") {
                    boost::asio::post(context_, boost::bind(&UserTalker::CreateGame, this));
                } else if (command == "join-room") {
                    boost::asio::post(context_, boost::bind(&UserTalker::JoinPlayer, this));
                } else if (command == "logout") {
                    boost::asio::post(context_, boost::bind(&UserTalker::Logout, this));
                } else if (command == "disconnect") {
                    boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
                } else {
                    boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
                }
            }
        }
    }
}

}  // namespace network
