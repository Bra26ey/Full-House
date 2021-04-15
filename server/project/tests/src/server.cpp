#include "tools.h"
#include "server.h"

TEST(Server, User) {
    boost::asio::io_context context;
    network::User user(context);
    boost::asio::streambuf buffer;
    std::ostream os(&buffer);
    os << "msg";
    ASSERT_TRUE(user.Connect());
    ASSERT_EQ(user.Send(buffer), 0);
    ASSERT_EQ(user.Read(buffer), 0);
    ASSERT_TRUE(user.Disconnect());
}
