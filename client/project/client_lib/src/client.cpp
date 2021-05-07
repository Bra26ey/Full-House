#include "client.h"

#include <string_view>
#include <iostream>

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

}  // namespace network