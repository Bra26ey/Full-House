#include <iostream>

#include "tools.h"
#include "client.h"

TEST(Client, Connection) {
    network::Client client;
    ASSERT_TRUE(client.Connect());
    ASSERT_TRUE(client.Disconnect());
}

TEST(Client, IsConnected) {
    network::Client client;
    ASSERT_FALSE(client.IsConnected());
}

TEST(Client, Read) {
    network::Client client;
    boost::asio::streambuf buffer;
    ASSERT_TRUE(client.Connect());
    ASSERT_EQ(client.Read(buffer), 0);
    ASSERT_TRUE(client.Disconnect());
}

TEST(Client, Write) {
    network::Client client;
    boost::asio::streambuf buffer;
    std::ostream os(&buffer);
    os << "msg";
    ASSERT_TRUE(client.Connect());
    ASSERT_EQ(client.Send(buffer), 0);
    ASSERT_TRUE(client.Disconnect());
}
