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
    auto ans = usr.RegUser("test_user", "test_password");
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
    auto ans = usr.RegUser("test_user2", "test_password2");
    ASSERT_EQ(ans.second, OK);
    USER_ID2 = ans.first;
    int out = usr.UpdateMoneyByDelta(USER_ID2, 11);
    ASSERT_EQ(out, OK);

    Board brd;
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID1, "1234"), WRONG_PASSWORD);
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID1, "12345678"), OK);
    ASSERT_EQ(brd.AddUserToBoard(BOARD_ID, USER_ID2, "12345678"), OK);
}

TEST(Board, change_admin) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.UpdateBoardAdmin(BOARD_ID, USER_ID2), OK);
}

TEST(Board, update_hand_configuration) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    hand_configuration_t hand_config;
    hand_config.button_pos = 1;
    hand_config.small_blind_pos = 2;
    hand_config.big_blind_pos = 3;
    hand_config.small_blind_bet = 4;
    hand_config.big_blind_bet = 5;
    hand_config.max_size_of_players = 6;
    hand_config.count_of_player_cards = 7;
    Board brd;
    ASSERT_EQ(brd.UpdateHandConfiguration(BOARD_ID, hand_config), OK);
}

TEST(Board, set_reserved_money) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.SetReservedMoney(BOARD_ID, USER_ID1, 25.0), INSUFFICIENT_FUNDS);
    ASSERT_EQ(brd.SetReservedMoney(BOARD_ID, USER_ID2, 10.0), OK);
}

TEST(Board, upd_user_position) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    ASSERT_EQ(brd.UpdateUserPosition(BOARD_ID, USER_ID1, 5), OK);
}

TEST(Board, active_board) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);

    Board brd;
    active_board_t act_board = brd.GetActiveBoard(BOARD_ID);
    ASSERT_EQ(act_board.status_code, OK);
    ASSERT_EQ(act_board.board_id, BOARD_ID);
    ASSERT_EQ(act_board.players[0].reserved_money, 0.0);
    ASSERT_EQ(act_board.players[1].reserved_money, 10.0);

    act_board.players[0].position = 3;
    act_board.hand_config.button_pos = 4;
    act_board.players[1].reserved_money = 50;
    ASSERT_EQ(brd.SaveActiveBoard(act_board), OK);
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