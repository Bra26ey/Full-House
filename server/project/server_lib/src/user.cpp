#include "user.h"

namespace network {

User::~User() {}

bool User::Connect() {
    return false;
}

bool User::Disconnect() {
    return false;
}

bool User::IsConnected() {
    return false;
}

int User::Send(boost::asio::streambuf &buffer) {
    return 0;
}

int User::Read(boost::asio::streambuf &buffer) {
    return 0;
}

}  // namespace network
