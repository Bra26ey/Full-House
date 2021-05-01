#include "client.h"

namespace network {

Client::~Client() {
    socket_.close();
}

bool Client::Connect() {
    socket_.async_connect()
}

bool Client::Disconnect() {
    return true;
}

bool Client::IsConnected() {
    return true;
}

int Client::Send(boost::asio::streambuf &buffer) {
    return 0;
}

int Client::Read(boost::asio::streambuf &buffer) {
    return 0;
}

}  // namespace network