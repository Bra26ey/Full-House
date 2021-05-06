#include "gtest/gtest.h"
#include "../Connection/Connection.h"
#include "../Connection/PoolConnections.h"
#include "../Models/Board.h"


std::size_t BOARD_ID = 0;
std::size_t USER_ID1 = 0;
std::size_t USER_ID2 = 0;


TEST(Board, create_board__by_user_wich_not_exist) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    std::pair<std::size_t, int> pr(0, OBJECT_NOT_EXIST);
    ASSERT_EQ(brd.CreateBoard(1, "12345678"), pr);
}

TEST(Board, create_board__ok) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;
    auto ans = usr.InsertUser("test_user", "test_password");
    ASSERT_EQ(ans.second, OK);
    USER_ID1 = ans.first;

    Board brd;
    std::pair<std::size_t, int> out = brd.CreateBoard(USER_ID1, "12345678");
    ASSERT_EQ(out.second, OK);
    BOARD_ID = out.first;
}

TEST(Board, get_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    board_t board_struct = brd.GetBoard(BOARD_ID, true);

    ASSERT_EQ(board_struct.status_code, OK);
    ASSERT_EQ(board_struct.password, "12345678");
    ASSERT_EQ(board_struct.board_id, BOARD_ID);
    ASSERT_EQ(board_struct.admin_id, USER_ID1);
}

TEST(Board, add_user_to_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    User usr;
    auto ans = usr.InsertUser("test_user2", "test_password2");
    ASSERT_EQ(ans.second, OK);
    USER_ID2 = ans.first;

    Board brd;
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID1, "1234"), WRONG_PASSWORD);
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID1, "12345678"), OK);
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID2, "12345678"), OK);
}

TEST(Board, set_reserved_money) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.SetReservedMoney(BOARD_ID, USER_ID1, 25.0), OK);
    ASSERT_EQ(brd.SetReservedMoney(BOARD_ID, USER_ID2, 10.0), OK);
}

TEST(Board, get_active_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    active_board_t act_board = brd.GetActiveBoard(BOARD_ID);
    ASSERT_EQ(act_board.status_code, OK);
    ASSERT_EQ(act_board.board_id, BOARD_ID);
    ASSERT_EQ(act_board.users[1].second, 10.0);
}

TEST(Board, remove_user_from_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.RemoveUserFromBoard(BOARD_ID, USER_ID2), OK);
}

TEST(Board, remove_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.DeleteBoard(BOARD_ID), OK);

    User usr;
    ASSERT_EQ(usr.DeleteUser(USER_ID1), OK);
    ASSERT_EQ(usr.DeleteUser(USER_ID2), OK);
}