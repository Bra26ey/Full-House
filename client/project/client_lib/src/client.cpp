#include "client.h"

#include <string_view>
#include <iostream>

#include "msgmaker.h"

using boost::asio::ip::address;

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
            out_ << MsgClient::Ping();
        } else {
            out_ << msg_queue_.Pop();
        }

        boost::asio::write(socket_, write_buffer_);

        boost::asio::read_until(socket_, read_buffer_, "\n\r\n\r");
    }
}

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