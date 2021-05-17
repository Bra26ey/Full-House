#include "gtest/gtest.h"
#include "../Connection/PoolConnections.h"
#include "../Models/User.h"


TEST(User, add_user) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    auto out = usr.RegUser("test_user", "test_password");
    ASSERT_EQ(out.second, OK);
}

TEST(User, is_exist) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    int out = usr.IsExist("test_user");
    ASSERT_EQ(out, true);
}

TEST(User, auth_user) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    auto out = usr.AuthUser("test_user", "test_password");
    ASSERT_EQ(out.second, OK);
    user_t user_struct = out.first;
    ASSERT_EQ(user_struct.login, "test_user");
    ASSERT_EQ(user_struct.password, "test_password");
}

TEST(User, get_user__in_database) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    user_t user_struct = usr.GetUser("test_user", false);
    ASSERT_EQ(user_struct.status_code, OK);
    ASSERT_EQ(user_struct.login, "test_user");
    ASSERT_EQ(user_struct.password, "");
}

TEST(User, get_user__not_in_database) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    user_t user_struct = usr.GetUser("not_test_user", false);
    ASSERT_FALSE(user_struct.status_code == OK);
}

TEST(User, update_params) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    user_t user_struct = usr.GetUser("test_user", false);
    ASSERT_EQ(user_struct.status_code, OK);

    ASSERT_EQ(usr.UpdateLogin(user_struct.id, "new_test_user"), OK);
    ASSERT_EQ(usr.UpdatePassword(user_struct.id, "new_test_password"), OK);
    ASSERT_EQ(usr.UpdateUsername(user_struct.id, "new_username"), OK);
    ASSERT_EQ(usr.UpdateAvatar(user_struct.id, "new_path_to_avatar"), OK);
    ASSERT_EQ(usr.UpdateMoney(user_struct.id, 25.0), OK);
    ASSERT_EQ(usr.UpdateMoneyByDelta(user_struct.id, -7.0), OK);

    user_struct = usr.GetUser("new_test_user", true);
    ASSERT_EQ(user_struct.status_code, OK);
    ASSERT_EQ(user_struct.login, "new_test_user");
    ASSERT_EQ(user_struct.password, "new_test_password");
    ASSERT_EQ(user_struct.username, "new_username");
    ASSERT_EQ(user_struct.avatar, "new_path_to_avatar");
    ASSERT_EQ(user_struct.money, 18.0);
}

TEST(User, delete_user) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;

    user_t user_struct = usr.GetUser("new_test_user", true);
    ASSERT_EQ(usr.DeleteUser(user_struct.id), OK);
}