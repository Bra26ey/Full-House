#include "client.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    network::Client client;
    boost::asio::streambuf write_buffer;
    boost::asio::streambuf read_buffer;
    std::ostream out(&write_buffer);
    std::string str;

    client.Connect();

    while (true) {
        getline(std::cin, str);
        out << "{" << str << "}" << "\n\r\n\r";
        client.Send(write_buffer);
        client.Read(read_buffer);
    }

    // out << "{login: Keny\npassword: omg}";
    // client.Send(write_buffer);
    // client.Read(read_buffer);

    // out << "{create: room}";
    // client.Send(write_buffer);
    // client.Read(read_buffer);

    return 0;
}