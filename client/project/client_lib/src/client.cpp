#include "client.h"

#include <string_view>
#include <iostream>

#include <boost/thread/thread.hpp>

#include "msgmaker.h"

using boost::asio::ip::address;

// namespace time = boost::posix_time;

constexpr uint32_t PING_TIME = 500;


// constexpr std::string_view SERVER_IP = "127.0.1.0";  // for local
constexpr std::string_view SERVER_IP = "89.19.190.83";  // for public
constexpr size_t SERVER_PORT = 5000;

namespace network {

Client::~Client() {
    socket_.close();
}

bool Client::Connect() {
    tcp::endpoint endpoint(address::from_string(std::string(SERVER_IP)), SERVER_PORT);
    boost::system::error_code error;
    socket_.connect(endpoint, error);
    if (error) {
        std::cout << error.message() << std::endl;
        return false;
    }

    is_closeing.store(false);
    std::cout << "connection done on ep = " << endpoint << std::endl;
    last_ping = boost::posix_time::microsec_clock::local_time();
    return true;
}

bool Client::Disconnect() {
    auto msg = MsgClient::Disconnect();
    msg_queue_.Push(msg);
    is_closeing.store(true);
    return true;
}

bool Client::IsConnected() {
    return socket_.is_open();
}

void Client::Run() {
    while (!is_closeing.load()) {
        if (msg_queue_.IsEmpty()) {
            // continue;
            auto delta = boost::posix_time::microsec_clock::local_time() - last_ping;
            if (delta.total_milliseconds() < PING_TIME) {
                continue;
            }
            out_ << MsgClient::Ping();
        } else {
            out_ << msg_queue_.Pop();
        }

        boost::asio::write(socket_, write_buffer_);

        boost::asio::read_until(socket_, read_buffer_, std::string(MSG_END));

        std::string answer(std::istreambuf_iterator<char>(in_), {});
        answers_queue_.Push(answer);

        last_ping = boost::posix_time::microsec_clock::local_time();
    }

    while (!msg_queue_.IsEmpty()) {
        out_ << msg_queue_.Pop();

        boost::asio::write(socket_, write_buffer_);

        boost::asio::read_until(socket_, read_buffer_, std::string(MSG_END));

        std::string answer(std::istreambuf_iterator<char>(in_), {});
        answers_queue_.Push(answer);

        last_ping = boost::posix_time::microsec_clock::local_time();
    }

    socket_.close();
}


std::string Client::GetLastMsg() {
    while (answers_queue_.IsEmpty()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }

    return answers_queue_.Pop();
}

    // void on_check_ping() {
    //     boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    //     if ( (now - last_ping).total_milliseconds() > 5000) {
    //         std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
    //         stop();
    //     }
    //     last_ping = boost::posix_time::microsec_clock::local_time();
    // }
    // void post_check_ping() {
    //     timer_.expires_from_now(boost::posix_time::millisec(5000));
    //     timer_.async_wait( MEM_FN(on_check_ping));
    // }

void Client::Autorise(std::string const &login, std::string const &password) {
    auto msg = MsgClient::Autorisation(login, password);
    msg_queue_.Push(msg);
}

void Client::Registrate(std::string const &login, std::string const &password) {
    auto msg = MsgClient::Registration(login, password);
    msg_queue_.Push(msg);
}

void Client::Logout() {
    auto msg = MsgClient::Logout();
    msg_queue_.Push(msg);
}

void Client::CreateRoom(std::string const &password) {
    auto msg = MsgClient::CreateRoom(password);
    msg_queue_.Push(msg);
    msg = MsgClient::CreateRoomResault();
    msg_queue_.Push(msg);
}

void Client::JoinRoom(uint64_t const &id, std::string const &password) {
    auto msg = MsgClient::JoinRoom(id, password);
    msg_queue_.Push(msg);
    msg = MsgClient::JoinRoomResult();
    msg_queue_.Push(msg);
}

void Client::StartGame() {
    auto msg = MsgClient::StartGame();
    msg_queue_.Push(msg);
}

void Client::LeaveRoom() {
    auto msg = MsgClient::LeaveRoom();
    msg_queue_.Push(msg);
}

void Client::GetGameStatus() {
    auto msg = MsgClient::GetGameStatus();
    msg_queue_.Push(msg);
}

void Client::GameRaise(uint64_t const &sum) {
    auto msg = MsgClient::GameRaise(sum);
    msg_queue_.Push(msg);
}

void Client::GameCall() {
    auto msg = MsgClient::GameCall();
    msg_queue_.Push(msg);
}

void Client::GameFold() {
    auto msg = MsgClient::GameFold();
    msg_queue_.Push(msg);
}

void Client::GameCheck() {
    auto msg = MsgClient::GameCheck();
    msg_queue_.Push(msg);
}

void Client::AddMoney(uint64_t const &sum) {
    auto msg = MsgClient::AddMoney(sum);
    msg_queue_.Push(msg);
}

void Client::GetMoneyInfo() {
    auto msg = MsgClient::MoneyInfo();
    msg_queue_.Push(msg);
}

}  // namespace network
