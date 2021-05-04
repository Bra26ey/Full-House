#include "gamehandler.h"

#include <boost/asio/yield.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

static void Clean(std::istream &in) {
    if (in.eof()) {
        return;
    }

    char c;
    in >> c;
    while (c != '}') {
        in >> c;
    }
}

namespace network {

GameHandler::~GameHandler() {}

void GameHandler::Start() {
    BOOST_LOG_TRIVIAL(info) << "start gamehandler";
    boost::asio::post(context_, boost::bind(&GameHandler::HandlePlayers, this));
}

void GameHandler::HandlePlayers() {
    if (!autorised_.IsEmpty()) {
        auto user = autorised_.Pop();
        boost::asio::post(context_, boost::bind(&GameHandler::HandleRequest, this, user));
        BOOST_LOG_TRIVIAL(info) << "try read request";
    }
    boost::asio::post(context_, boost::bind(&GameHandler::HandlePlayers, this));
}

void GameHandler::CreateGame(std::shared_ptr<User> user) {
    Clean(user->in);  // TODO(ANDY) insted add reading & handling params of the room
    auto gameroom = std::make_shared<GameRoom>(context_);
    gameroom->AddUser(user);
    gamerooms.push_back(gameroom);
    user->out << "{room-id: " << 0 << "; room-pass: " << 0 << ";}";
    BOOST_LOG_TRIVIAL(info) << "game created. room-id: " << 0;
    async_write(user->socket_, user->write_buffer, boost::bind(&game_queue::Push, &waiting_start_, gameroom));
}


void GameHandler::JoinPlayer(std::shared_ptr<User> user) {
    Clean(user->in);  // TODO(ANDY) insted add reading & handling params of the room
    gamerooms[0]->AddUser(user);
    user->out << "{status: accepted; room-id: " << 0 << ";}";
    BOOST_LOG_TRIVIAL(info) << user->name_ << " accepted to game. room-id: " << 0;
    write(user->socket_, user->write_buffer);
}

void GameHandler::HandleError(std::shared_ptr<User> user) {
    Clean(user->in);
    user->out << "{error: unknown format;}";
    BOOST_LOG_TRIVIAL(info) << user->name_ << "'s request is anknown. return user to autorisator";
    async_write(user->socket_, user->write_buffer, boost::bind(&user_queue::Push, &autorised_, user));
}

void GameHandler::HandleRequest(std::shared_ptr<User> user) {
    reenter(this) {
        while (true) {
            yield async_read_until(user->socket_, user->read_buffer, '}', boost::bind(&GameHandler::HandleRequest, this, user));
            yield {
                std::string str;
                user->in >> str;
                BOOST_LOG_TRIVIAL(info) << "handle user request: " << str;
                if (str == "{create:") {
                    boost::asio::post(context_, boost::bind(&GameHandler::CreateGame, this, user));
                } else if (str == "{join:") {
                    boost::asio::post(context_, boost::bind(&GameHandler::JoinPlayer, this, user));
                } else {
                    boost::asio::post(context_, boost::bind(&GameHandler::HandleError, this, user));
                }
            }
        }
    }
}

// void GameHandler::DoGame(std::shared_ptr<GameRoom> gameroom) {
    
// }

void GameHandler::DoGame(std::shared_ptr<GameRoom> gameroom) {
    BOOST_LOG_TRIVIAL(info) << "game started";
}

}  // namespace network