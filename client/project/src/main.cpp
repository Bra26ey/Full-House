#include "client.h"

#include <iostream>
#include <thread>

network::Client client;

void print_msg() {
    while(client.IsConnected()) {
        std::cout << client.GetLastMsg();
    }
}

int main(int argc, const char *argv[]) {
    client.Connect();

    std::thread t(&network::Client::Run, &client);
    t.detach();

    std::thread m(print_msg);
    m.detach();

    while (client.IsConnected()) {
        int code;
        std::cin >> code;
        std::string login;
        std::string password;
        switch (code) {
        case 0:
            client.GameFold();
            break;
        case 1:
            client.GameCall();
            break;
        case 2:
            uint64_t sum;
            std::cin >> sum;
            client.GameRaise(sum);
            break;
        case 3:
            client.GameCheck();
            break;
        case 4:
            std::cin >> password;
            client.CreateRoom(password);
            break;
        case 5:
            uint64_t id;
            std::cin >> id;
            std::cin >> password;
            client.JoinRoom(id, password);
            break;
        case 6:
            client.StartGame();
            break;
        case 7:
            client.GetGameStatus();
            break;
        case 8:
            client.LeaveRoom();
            break;
        case 9:
            client.Logout();
            break;
        case 10:
            client.Disconnect();
            break;
        case 11:
            std::cin >> login >> password;
            client.Autorise(login, password);
            break;
        case 12:
            std::cin >> login >> password;
            client.Registrate(login, password);
            break;
        default:
            break;
        }
    }


    return 0;
}

