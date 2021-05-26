#include "usertalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "msgmaker.h"

using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace network {

UserTalker::UserTalker(std::shared_ptr<User> &user, Userbase &userbase, database::User &user_db)
           : is_remove(false),
             context_(user->context),
             user_(user),
             userbase_(userbase),
             user_db_(user_db) {}

void UserTalker::Start() {
    user_->is_talking.store(true);
    BOOST_LOG_TRIVIAL(info) << "UserTalker start work with connection";
    boost::asio::post(context_, boost::bind(&UserTalker::OnHandleRequest, this));
}

bool UserTalker::IsUserWorks() const {
    return (user_->is_talking.load() || user_->is_gaming.load());
}

void UserTalker::HandleRegistration() {
    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    auto name = parametrs.get<std::string>("login");
    auto password = parametrs.get<std::string>("password");

    auto answer = user_db_.RegUser(name, password);

    if (answer.second == database::OK) {
        user_->out << MsgServer::RegistrationDone();
        BOOST_LOG_TRIVIAL(info) << "registration: done. name: " << user_->name;
    } else {
        user_->out << MsgServer::RegistrationFailed();
        BOOST_LOG_TRIVIAL(info) << "registration: failed";
    }

    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::HandleAutorisation() {
    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    auto name = parametrs.get<std::string>("login");
    auto password = parametrs.get<std::string>("password");

    auto answer = user_db_.AuthUser(name, password);
    user_->is_autorised = answer.second == database::OK;

    if (user_->is_autorised) {
        user_->name = name;
        user_->id = answer.first.id;
        user_->money = static_cast<uint64_t>(answer.first.money);
        user_->out << MsgServer::AutorisationDone();
        BOOST_LOG_TRIVIAL(info) << "user is autorised. name: " << user_->name;
    } else {
        user_->out << MsgServer::AutorisationFailed();
        BOOST_LOG_TRIVIAL(info) << "autorisation: invalid data";
    }

    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::CreateGame() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to create room";
    user_->out << MsgServer::CreateRoomOn();
    async_write(user_->socket, user_->write_buffer, [this](bs::error_code error, size_t len) {
        if (!error) {
            userbase_.creating_game.Push(user_);
            // boost::bind(&user_queue::Push, &userbase_.creating_game, user_);
        } else {
            // boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
            Disconnect();
        }
    });
}


void UserTalker::JoinPlayer() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to accept game";

    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    user_->room_id = parametrs.get<uint64_t>("id");
    BOOST_LOG_TRIVIAL(info) << user_->room_id;

    user_->out << MsgServer::JoinRoomOn(user_->room_id);
    async_write(user_->socket, user_->write_buffer, [this](bs::error_code error, size_t len) {
        if (!error) {
            userbase_.accepting_game.Push(user_);
            // boost::bind(&user_queue::Push, &userbase_.accepting_game, user_);
        } else {
            // boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
            Disconnect();
        }
    });
}

void UserTalker::HandleError() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "'s request is unknown";

    user_->out << MsgServer::Error();
    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::HandlePing() {
    user_->out << MsgServer::Ping();
    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::Disconnect() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "is disconnected";

    user_->out << MsgServer::Disconnect();
    async_write(user_->socket, user_->write_buffer, [this](bs::error_code error, size_t len) {
        user_->socket.close();
        is_remove.store(true);
    });
}

void UserTalker::HandleAddMoney() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "is hungry for money";

    const pt::ptree &parametrs = user_->last_msg.get_child("parametrs");
    auto sum = parametrs.get<uint64_t>("money");

    auto code = user_db_.UpdateMoneyByDelta(user_->id, sum);

    if (code == database::OK) {
        user_->out << MsgServer::AddMoneyDone();
        user_->money += sum;
    } else {
        user_->out << MsgServer::AddMoneyFailed();
    }

    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::HandleMoneyInfo() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " asking for money";

    user_->out << MsgServer::MoneyInfo(user_->money);

    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::Logout() {
    BOOST_LOG_TRIVIAL(info) << user_->name << "is loged out";

    user_->name.clear();
    user_->is_autorised = false;

    user_->out << MsgServer::Logout();
    boost::asio::post(context_, boost::bind(&UserTalker::SendAnswer, this));
}

void UserTalker::OnHandleRequest() {
    async_read_until(user_->socket, user_->read_buffer, std::string(MSG_END), [this](bs::error_code error, size_t len) {
        if (!error) {
            HandleRequest();
            // boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
        } else {
            // add timer here
            OnHandleRequest();
            // boost::asio::post(context_, boost::bind(&UserTalker::OnHandleRequest, this));
            // Disconnect();
        }
    });
}

void UserTalker::SendAnswer() {
    async_write(user_->socket, user_->write_buffer, [this](bs::error_code error, size_t len) {
        if (!error) {
            // boost::asio::post(context_, boost::bind(&UserTalker::OnHandleRequest, this));
            OnHandleRequest();
        } else {
            // boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
            Disconnect();
        }
    }); 
}

void UserTalker::HandleRequest() {
    pt::read_json(user_->in, user_->last_msg);
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

    if (command == "disconnect") {
        boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
        return;
    }

    if (!user_->is_autorised) {
        if (command == "autorisation") {
            boost::asio::post(context_, boost::bind(&UserTalker::HandleAutorisation, this));
        } else if (command == "registration") {
            boost::asio::post(context_, boost::bind(&UserTalker::HandleRegistration, this));
        } else {
            boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
        }
        return;
    }

    if (command == "create-room") {
        boost::asio::post(context_, boost::bind(&UserTalker::CreateGame, this));
    } else if (command == "add-money") {
        boost::asio::post(context_, boost::bind(&UserTalker::HandleAddMoney, this));
    } else if (command == "money-info") {
        boost::asio::post(context_, boost::bind(&UserTalker::HandleMoneyInfo, this));
    } else if (command == "join-room") {
        boost::asio::post(context_, boost::bind(&UserTalker::JoinPlayer, this));
    } else if (command == "logout") {
        boost::asio::post(context_, boost::bind(&UserTalker::Logout, this));
    } else {
        boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
    }
}

}  // namespace network
