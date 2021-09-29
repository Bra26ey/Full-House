#include "tools.h"
#include "server.h"

constexpr size_t QUEUE_SIZE = 4;

// TEST(Server, User) {
//     boost::asio::io_context context;
//     network::User user(context);
//     boost::asio::streambuf buffer;
//     std::ostream os(&buffer);
//     os << "msg";
//     // ASSERT_EQ(user.Connect(), 0);
//     ASSERT_EQ(user.Send(buffer), 0);
//     ASSERT_EQ(user.Read(buffer), 0);
//     ASSERT_TRUE(user.Disconnect());
// }

TEST(Server, UserQueue) {
    boost::asio::io_context context;
    network::user_queue users;
    for (size_t i = 0; i < QUEUE_SIZE; ++i) {
        auto user = std::make_shared<network::User>(context);
        users.Push(user);
    }
    ASSERT_FALSE(users.IsEmpty());
    for (size_t i = 0; i < QUEUE_SIZE; ++i) {
        users.Pop();
    }
    ASSERT_TRUE(users.IsEmpty());
}

// TEST(Server, GameRoom) {
//     boost::asio::io_context context;
//     network::GameRoom gameroom(context);
//     for (size_t i = 0; i < QUEUE_SIZE; ++i) {
//         auto user = std::make_shared<network::User>(context);
//         gameroom.AddUser(user);
//     }
//     gameroom.Step();
// }

// TEST(Server, GameHandler) {
//     boost::asio::io_context context;
//     network::Userbase userbase;
//     for (size_t i = 0; i < QUEUE_SIZE; ++i) {
//         auto user = std::make_shared<network::User>(context);
//         userbase.waitng_autorisation.Push(user);

//     }
//     for (size_t i = 0; i < QUEUE_SIZE; ++i) {
//         auto user = std::make_shared<network::User>(context);
//         userbase.autorised.Push(user);

//     }
//     for (size_t i = 0; i < QUEUE_SIZE; ++i) {
//         auto user = std::make_shared<network::User>(context);
//         userbase.creating_game.Push(user);

//     }
//     for (size_t i = 0; i < QUEUE_SIZE; ++i) {
//         auto user = std::make_shared<network::User>(context);
//         userbase.accepting_game.Push(user);

//     }

//     ASSERT_NO_THROW(network::GameHandler gamehandler(context, userbase));
// }

TEST(Server, Server) {
    ASSERT_NO_THROW(network::Server server);
}