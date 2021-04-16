#include "user.h"

namespace network {

User::~User() {}

bool User::Connect() {}

bool User::Disconnect() {}

bool User::IsConnected() {}

int User::Send(boost::asio::streambuf &buffer) {}

int User::Read(boost::asio::streambuf &buffer) {}

}  // namespace network
