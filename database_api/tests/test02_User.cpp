#include "gtest/gtest.h"
#include "../Connection/PoolConnections.h"
#include "../Models/User.h"


Connection* GetConn() {
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("localhost", "****", "****", "poker_database");
    return instance->GetConnection();
}


TEST(User, add_user) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    int out = usr.InsertUser("test_user", "test_password");
    ASSERT_EQ(out, OK);

    conn->Close();
}

TEST(User, is_exist) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    int out = usr.IsExist("test_user");
    ASSERT_EQ(out, true);

    conn->Close();
}

TEST(User, get_user__in_database) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    user_t user_struct = usr.GetUser("test_user", false);
    ASSERT_EQ(user_struct.status_code, OK);
    ASSERT_EQ(user_struct.login, "test_user");
    ASSERT_EQ(user_struct.password, "");

    conn->Close();
}

TEST(User, get_user__not_in_database) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    user_t user_struct = usr.GetUser("not_test_user", false);
    ASSERT_FALSE(user_struct.status_code == OK);

    conn->Close();
}

TEST(User, update_params) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    ASSERT_EQ(usr.UpdateLogin("test_user", "new_test_user"), OK);
    ASSERT_EQ(usr.UpdatePassword("new_test_user", "new_test_password"), OK);
    ASSERT_EQ(usr.UpdateUsername("new_test_user", "new_username"), OK);
    ASSERT_EQ(usr.UpdateAvatar("new_test_user", "new_path_to_avatar"), OK);
    ASSERT_EQ(usr.UpdateMoney("new_test_user", 25.0), OK);

    user_t user_struct = usr.GetUser("new_test_user", true);
    ASSERT_EQ(user_struct.status_code, OK);
    ASSERT_EQ(user_struct.login, "new_test_user");
    ASSERT_EQ(user_struct.password, "new_test_password");
    ASSERT_EQ(user_struct.username, "new_username");
    ASSERT_EQ(user_struct.avatar, "new_path_to_avatar");
    ASSERT_EQ(user_struct.money, 25.0);

    conn->Close();
}

TEST(User, delete_user) {
    Connection* conn = GetConn();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);

    ASSERT_EQ(usr.DeleteUser("new_test_user"), OK);

    conn->Close();
}