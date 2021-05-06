#include "usertalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

static void Clean(std::istream &in) {
    in.clear();
    char c = in.peek();
    while (c != EOF) {
        in >> c;
        c = in.peek();
    }
    in.clear();
}

namespace network {

void UserTalker::Start() {
    // boost::property_tree::ptree p;
    user_->is_talking.store(true);
    BOOST_LOG_TRIVIAL(info) << "UserTalker start work with connection";
    boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
}

bool UserTalker::IsUserWorks() const {
    return (user_->is_talking.load() || user_->is_gaming.load());
}

void UserTalker::HandleAutorisation() {
    user_->is_autorised = true;
    if (user_->is_autorised) {
        user_->in >> user_->name;
        user_->out << "{ok}";
        BOOST_LOG_TRIVIAL(info) << "user is autorised. name: " << user_->name;
    } else {
        user_->out << "{not ok}";
        BOOST_LOG_TRIVIAL(info) << "autorisation: invalid data";
    }
    Clean(user_->in);
    write(user_->socket, user_->write_buffer);
    boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::CreateGame() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to create room";
    Clean(user_->in);  // TODO(ANDY) insted add reading & handling params of the room
    user_->out << "{status: trying to create;}";
    write(user_->socket, user_->write_buffer);
    userbase_.creating_game.Push(user_);
}


void UserTalker::JoinPlayer() {
    BOOST_LOG_TRIVIAL(info) << user_->name << " trying to accept game";
    user_->in >> user_->room_id;
    Clean(user_->in);
    user_->out << "{status: trying to accept; room-id: " << user_->room_id << ";}";
    write(user_->socket, user_->write_buffer);
    userbase_.accepting_game.Push(user_);
}

void UserTalker::HandleError() {
    Clean(user_->in);
    user_->out << "{error: unknown format;}";
    write(user_->socket, user_->write_buffer);
    BOOST_LOG_TRIVIAL(info) << user_->name << "'s request is anknown";
    boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::Disconnect() {
    Clean(user_->in);
    user_->out << "{status: disconnected;}";
    BOOST_LOG_TRIVIAL(info) << user_->name << "is disconnected";
    write(user_->socket, user_->write_buffer);
    is_remove.store(true);
}

void UserTalker::Logout() {
    Clean(user_->in);
    user_->name.clear();
    user_->is_autorised = false;
    user_->out << "{status: notloged;}";
    BOOST_LOG_TRIVIAL(info) << user_->name << "is loged out";
    write(user_->socket, user_->write_buffer);
    boost::asio::post(context_, boost::bind(&UserTalker::HandleRequest, this));
}

void UserTalker::HandleRequest() {
    reenter(this) {
        while (true) {
            // if (user_->is_autorised) {
            //     BOOST_LOG_TRIVIAL(info) << user_->name << " is ready for new request";
            // } else {
            //     BOOST_LOG_TRIVIAL(info) << "unloged user is ready for new request";
            // }
            yield async_read_until(user_->socket, user_->read_buffer, "\n\r\n\r", boost::bind(&UserTalker::HandleRequest, this));
            yield {
                std::string str;
                user_->in >> str;
                BOOST_LOG_TRIVIAL(info) << "handle user request: " << str;

                if (!user_->is_autorised) {
                    if (str == "{login:") {
                        boost::asio::post(context_, boost::bind(&UserTalker::HandleAutorisation, this));
                    } else {
                        boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
                    }
                    return;
                }

                if (str == "{create:") {
                    boost::asio::post(context_, boost::bind(&UserTalker::CreateGame, this));
                } else if (str == "{join:") {
                    boost::asio::post(context_, boost::bind(&UserTalker::JoinPlayer, this));
                } else if (str == "{logout:") {
                    boost::asio::post(context_, boost::bind(&UserTalker::Logout, this));
                } else if (str == "{disconnect:") {
                    boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
                } else {
                    boost::asio::post(context_, boost::bind(&UserTalker::HandleError, this));
                }
            }
            // yield async_write(user_->socket, user_->write_buffer, boost::bind(&UserTalker::HandleRequest, this));
        }
    }
}

}  // namespace network
