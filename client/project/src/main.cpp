// #include "client.h"
// #include "msgmaker.h"

// #include <boost/property_tree/json_parser.hpp>

// #include <iostream>

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<std::string>("std::string");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/other_textures/icon"));
    w.show();
    return a.exec();
}


// int main(int argc, const char *argv[]) {
//     MsgClient msgfabric;
//     // std::cout << msgfabric.Autorisation("Mike", "Password");
//     // std::cout << msgfabric.CreateRoom("Password");
//     // std::cout << msgfabric.Disconnect();

//     // std::cout << msgfabric.JoinRoom(0, "password");
//     // std::cout << msgfabric.JoinRoomResault();
//     // std::cout << msgfabric.CreateRoom("password");
//     // std::cout << msgfabric.CreateRoomResault();

//     boost::asio::streambuf buffer;
//     std::iostream is(&buffer);
//     is << msgfabric.CreateRoomResault();

//     boost::property_tree::ptree pt;
//     boost::property_tree::read_json(is, pt);

//     network::Client client;
//     boost::asio::streambuf write_buffer;
//     boost::asio::streambuf read_buffer;
//     std::ostream out(&write_buffer);
//     // std::string str;

//     client.Connect();

//     // while (true) {
//     //     getline(std::cin, str);
//     //     out << "{" << str << "}" << "\n\r\n\r";
//     //     client.Send(write_buffer);
//     //     client.Read(read_buffer);
//     // }

//     out << msgfabric.Autorisation("Mike", "Password");
//     client.Send(write_buffer);
//     client.Read(read_buffer);
//     sleep(1);

//     out << msgfabric.Logout();
//     client.Send(write_buffer);
//     client.Read(read_buffer);
//     sleep(1);

//     out << msgfabric.Autorisation("Nick", "Password");
//     client.Send(write_buffer);
//     client.Read(read_buffer);
//     sleep(1);

    

//     while (true) {
//         int code;
//         std::cin >> code;
//         switch (code) {
//         case 0:
//             out << msgfabric.GameFold();
//             break;
//         case 1:
//             out << msgfabric.GameCall();
//             break;
//         case 2:
//             uint64_t sum;
//             std::cin >> sum;
//             out << msgfabric.GameRaise(sum);
//             break;
//         case 3:
//             out << msgfabric.GameCheck();
//             break;
//         case 4:
//             out << msgfabric.CreateRoom("password");
//             client.Send(write_buffer);
//             client.Read(read_buffer);
//             out << msgfabric.CreateRoomResault();
//             break;
//         case 5:
//             uint64_t id;
//             std::cin >> id;
//             out << msgfabric.JoinRoom(id, "password");
//             client.Send(write_buffer);
//             client.Read(read_buffer);
//             out << msgfabric.JoinRoomResault();
//             break;
//         case 6:
//             out << msgfabric.StartGame();
//             break;
//         case 7:
//             out << msgfabric.GetGameStatus();
//             break;
//         default:
//             break;
//         }
//         client.Send(write_buffer);
//         client.Read(read_buffer);
//     }

//     // out << msgfabric.CreateRoom("Password");
//     // client.Send(write_buffer);
//     // client.Read(read_buffer);
//     // sleep(1);

//     // out << msgfabric.CreateRoomReault();
//     // client.Send(write_buffer);
//     // client.Read(read_buffer);
//     // sleep(1);

//     // out << msgfabric.LeaveRoom();
//     // client.Send(write_buffer);
//     // client.Read(read_buffer);
//     // sleep(1);

//     // out << msgfabric.Disconnect();
//     // client.Send(write_buffer);
//     // client.Read(read_buffer);
//     // sleep(1);

//     client.Disconnect();

//     return 0;
// }
