#include "gtest/gtest.h"
#include "../Connection/Connection.h"
#include "../Connection/PoolConnections.h"
#include "../Models/Board.h"


Connection* GetCon() {
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("localhost", "****", "****", "poker_database");
    return instance->GetConnection();
}

std::size_t BOARD_ID = 0;


TEST(Board, create_board__by_user_wich_not_exist) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    Board brd = Board(conn);
    std::pair<std::size_t, int> pr(0, OBJECT_NOT_EXIST);
    ASSERT_EQ(brd.CreateBoard("test_user"), pr);

    conn->Close();
}

TEST(Board, create_board__ok) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);
    ASSERT_EQ(usr.InsertUser("test_user", "test_password"), OK);

    Board brd = Board(conn);
    std::pair<std::size_t, int> out = brd.CreateBoard("test_user");
    ASSERT_EQ(out.second, OK);
    BOARD_ID = out.first;

    conn->Close();
}

TEST(Board, add_user_to_board) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    User usr = User(conn);
    ASSERT_EQ(usr.InsertUser("test_user2", "test_password2"), OK);

    Board brd = Board(conn);
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, "test_user2"), OK);

    conn->Close();
}

TEST(Board, set_reserved_money) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    Board brd = Board(conn);
    ASSERT_EQ(brd.SetReservedMoney(BOARD_ID, "test_user2", 25.0), OK);

    conn->Close();
}

TEST(Board, get_board) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    Board brd = Board(conn);
    board_t board_struct = brd.GetBoard(BOARD_ID);
    ASSERT_EQ(board_struct.status_code, OK);
    ASSERT_EQ(board_struct.board_id, BOARD_ID);
    ASSERT_EQ(board_struct.users[1].second, 25.0);

    conn->Close();
}

TEST(Board, remove_user_from_board) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    Board brd = Board(conn);
    ASSERT_EQ(brd.RemoveUserFromBoard(BOARD_ID, "test_user2"), OK);

    conn->Close();
}

TEST(Board, remove_board) {
    Connection* conn = GetCon();

    ASSERT_FALSE(conn == nullptr);
    ASSERT_EQ(conn->IsOpen(), true);

    Board brd = Board(conn);
    ASSERT_EQ(brd.DeleteBoard(BOARD_ID), OK);

    User usr = User(conn);
    ASSERT_EQ(usr.DeleteUser("test_user"), OK);
    ASSERT_EQ(usr.DeleteUser("test_user2"), OK);

    conn->Close();
}
