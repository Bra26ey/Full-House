#include "client.h"

#include <string_view>
#include <iostream>

#include "msgmaker.h"

using boost::asio::ip::address;

// namespace time = boost::posix_time;

constexpr uint32_t PING_TIME = 500;

// constexpr std::string_view SERVER_IP = "127.0.0.1";
constexpr size_t SERVER_PORT = 2000;

namespace network {

Client::~Client() {
    socket_.close();
}

bool Client::Connect() {
    // tcp::endpoint endpoint(address::from_string(SERVER_IP), SERVER_PORT);
    std::cout << "try connect" << std::endl;
    tcp::endpoint endpoint(address::from_string("127.0.1.1"), SERVER_PORT);
    socket_.connect(endpoint);
    std::cout << "connection done on ep = " << endpoint << std::endl;
    last_ping = boost::posix_time::microsec_clock::local_time();
    return true;
}

bool Client::Disconnect() {
    socket_.close();
    return true;
}

bool Client::IsConnected() {
    return socket_.is_open();
}

int Client::Send(boost::asio::streambuf &buffer) {
    boost::asio::write(socket_, buffer);
    std::cout << "send msg to server" << std::endl;
    return 0;
}

int Client::Read(boost::asio::streambuf &buffer) {
    boost::asio::read_until(socket_, buffer, "}");
    std::cout << "msg from server: " << &buffer << std::endl;
    return 0;
}

void Client::Run() {
    while (true) {
        if (msg_queue_.IsEmpty()) {
            auto delta = boost::posix_time::microsec_clock::local_time() - last_ping;
            if (delta.total_milliseconds() < PING_TIME) {
                continue;
            }
            out_ << MsgClient::Ping();
        } else {
            out_ << msg_queue_.Pop();
        }

        boost::asio::write(socket_, write_buffer_);

        boost::asio::read_until(socket_, read_buffer_, "\n\r\n\r");

        last_ping = boost::posix_time::microsec_clock::local_time();
    }
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
    msg = MsgClient::JoinRoomResault();
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

}  // namespace network