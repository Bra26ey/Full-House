#include "client.h"

namespace network {

Client::~Client() {}

bool Client::Connect() {
    return true;
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