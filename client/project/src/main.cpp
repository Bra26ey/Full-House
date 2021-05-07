#include "client.h"
#include "msgmaker.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    MsgClient msgfabric;
    // std::cout << msgfabric.Autorisation("Mike", "Password");
    // std::cout << msgfabric.CreateRoom("Password");
    // std::cout << msgfabric.Disconnect();

    network::Client client;
    boost::asio::streambuf write_buffer;
    boost::asio::streambuf read_buffer;
    std::ostream out(&write_buffer);
    // std::string str;

    client.Connect();

    // while (true) {
    //     getline(std::cin, str);
    //     out << "{" << str << "}" << "\n\r\n\r";
    //     client.Send(write_buffer);
    //     client.Read(read_buffer);
    // }

    out << msgfabric.Autorisation("Mike", "Password");
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.Logout();
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.Autorisation("Nick", "Password");
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.CreateRoom("Password");
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.CreateRoomReault();
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.LeaveRoom();
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    out << msgfabric.Disconnect();
    client.Send(write_buffer);
    client.Read(read_buffer);
    sleep(1);

    client.Disconnect();

    return 0;
}