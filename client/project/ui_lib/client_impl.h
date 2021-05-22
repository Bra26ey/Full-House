#ifndef CLIENT_H
#define CLIENT_H
#include "client.h"
#include <iostream>

inline std::unique_ptr<network::Client> Client;
namespace globalInfo {
    inline std::string Nickname = "";
    inline size_t Balance = 0;
}


#endif // CLIENT_H
