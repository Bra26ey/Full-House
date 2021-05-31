#include "Board.h"

namespace database {

board_t Board::GetBoard(const std::size_t &board_id, bool with_password = false) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    board_t b;

    if (!conn_.GetConnection().IsOpen()) {
        b.status_code = DATABASE_NOT_CONNECTED;
        return b;
    }

    std::string query_string = "SELECT * FROM board WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, (int) board_id);

    try {
        res.reset(pstmt->executeQuery());
        std::pair<std::size_t, double> pr(0, 0.0);
        if (res->next()) {
            b.board_id = res->getInt("id");
            b.admin_id = res->getInt("admin_id");
            if (with_password) {
                b.password = res->getString("password");
            }
            else {
                b.password = "";
            }
            b.status_code = OK;
        }
        else {
            b.status_code = EMPTY_DATA;
        }
    } catch (sql::SQLException &e) {
        b.status_code = OBJECT_NOT_EXIST;
        return b;
    }
    return b;
}


active_board_t Board::GetActiveBoard(const std::size_t &board_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    active_board_t act_board;

    if (!conn_.GetConnection().IsOpen()) {
        act_board.status_code = DATABASE_NOT_CONNECTED;
        return act_board;
    }

    std::string query_string = "SELECT user.id, user.username, user.avatar, active_board.reserved_money, active_board.position, "
                               "board.button_pos, board.small_blind_pos, board.big_blind_pos, board.small_blind_bet, board.big_blind_bet, "
                               "board.max_size_of_players, board.count_of_player_cards FROM user JOIN active_board ON user.id = "
                               "active_board.player_id JOIN board ON active_board.board_id = board.id WHERE board.id = ?;";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(board_id));

    try {
        res.reset(pstmt->executeQuery());
        player_t cur_player;
        if (res->next()) {
            act_board.board_id = board_id;
            cur_player.id = res->getInt("id");
            cur_player.username = res->getString("username");
            cur_player.avatar = res->getString("avatar");
            cur_player.reserved_money = static_cast<double>(res->getDouble("reserved_money"));
            cur_player.position = res->getInt("position");
            act_board.players.push_back(cur_player);
            act_board.hand_config.button_pos = res->getInt("button_pos");
            act_board.hand_config.small_blind_pos = res->getInt("small_blind_pos");
            act_board.hand_config.big_blind_pos = res->getInt("big_blind_pos");
            act_board.hand_config.small_blind_bet = res->getInt("small_blind_bet");
            act_board.hand_config.big_blind_bet = res->getInt("big_blind_bet");
            act_board.hand_config.max_size_of_players = res->getInt("max_size_of_players");
            act_board.hand_config.count_of_player_cards = res->getInt("count_of_player_cards");
        }
        while (res->next()) {
            cur_player.id = res->getInt("id");
            cur_player.username = res->getString("username");
            cur_player.avatar = res->getString("avatar");
            cur_player.reserved_money = static_cast<double>(res->getDouble("reserved_money"));
            cur_player.position = res->getInt("position");
            act_board.players.push_back(cur_player);
        }
        act_board.status_code = OK;
    } catch (sql::SQLException &e) {
        act_board.status_code = OBJECT_NOT_EXIST;
        return act_board;
    }

    return act_board;
}


int Board::SaveActiveBoard(const active_board_t &active_brd) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }
    // update board table
    std::string query_string = "UPDATE board SET button_pos = ?, small_blind_pos = ?, big_blind_pos = ?, small_blind_bet = ?, "
                               "big_blind_bet = ?, max_size_of_players = ?, count_of_player_cards = ? WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(active_brd.hand_config.button_pos));
    pstmt->setInt(2, static_cast<int>(active_brd.hand_config.small_blind_pos));
    pstmt->setInt(3, static_cast<int>(active_brd.hand_config.big_blind_pos));
    pstmt->setInt(4, static_cast<int>(active_brd.hand_config.small_blind_bet));
    pstmt->setInt(5, static_cast<int>(active_brd.hand_config.big_blind_bet));
    pstmt->setInt(6, static_cast<int>(active_brd.hand_config.max_size_of_players));
    pstmt->setInt(7, static_cast<int>(active_brd.hand_config.count_of_player_cards));
    pstmt->setInt(8, static_cast<int>(active_brd.board_id));

    // we admit that the system is not updatable
    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    // update active_board table
    query_string = "UPDATE active_board SET position = ?, reserved_money = ? WHERE board_id = ? AND player_id = ?;";
    for (auto & player : active_brd.players) {
        pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
        pstmt->setInt(   1, player.position);
        pstmt->setDouble(2, player.reserved_money);
        pstmt->setInt(   3, static_cast<int>(active_brd.board_id));
        pstmt->setInt(   4, static_cast<int>(player.id));
        try {
            pstmt->executeUpdate();
        } catch (sql::SQLException &e) {
            return OBJECT_NOT_EXIST;
        }
    }
    return OK;
}


std::pair<std::size_t, int> Board::CreateBoard(const std::size_t &user_id, const std::string& password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return std::pair<std::size_t, int> (0, DATABASE_NOT_CONNECTED);
    }

    std::string query_string = "INSERT INTO board(admin_id, password) VALUES (?, ?)";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(user_id));
    pstmt->setString(2, password);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        return std::pair<std::size_t, int> (0, OBJECT_NOT_EXIST);
    }

    query_string = "SELECT MAX(id) from board";
    std::shared_ptr<sql::Statement> stmt;
    stmt.reset(conn_.GetConnection().SetQuery(query_string));
    std::shared_ptr<sql::ResultSet> res;
    res.reset(stmt->executeQuery(query_string));
    std::size_t last_id = 0;
    if (res->next()) {
        last_id = res->getInt(1);
    }

    if (last_id != 0) {
        return std::pair<std::size_t, int>(last_id, OK);
    }
    else {
        return std::pair<std::size_t, int>(last_id, BAD_QUERY);
    }
}


int Board::UpdateBoardAdmin(const std::size_t& board_id, const std::size_t& new_admin_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    User usr;
    auto user_out = usr.GetUser(new_admin_id, false);
    if (user_out.status_code != OK) {
        return static_cast<int>(user_out.status_code);
    }

    std::string query_string = "UPDATE board SET admin_id = ? WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(new_admin_id));
    pstmt->setInt(2, static_cast<int>(board_id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}


int Board::DeleteBoard(const std::size_t &board_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM board WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(board_id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}


int Board::UpdateHandConfiguration(const std::size_t &board_id, const hand_configuration_t &hand_config) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE board SET button_pos = ?, small_blind_pos = ?, big_blind_pos = ?, small_blind_bet = ?, "
                               "big_blind_bet = ?, max_size_of_players = ?, count_of_player_cards = ? WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(hand_config.button_pos));
    pstmt->setInt(2, static_cast<int>(hand_config.small_blind_pos));
    pstmt->setInt(3, static_cast<int>(hand_config.big_blind_pos));
    pstmt->setInt(4,       hand_config.small_blind_bet);
    pstmt->setInt(5,       hand_config.big_blind_bet);
    pstmt->setInt(6, static_cast<int>(hand_config.max_size_of_players));
    pstmt->setInt(7, static_cast<int>(hand_config.count_of_player_cards));
    pstmt->setInt(8, static_cast<int>(board_id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}


int Board::CheckPassword(const std::size_t &board_id, const std::string &password) {
    std::string query_string = "SELECT password FROM board WHERE id = ?";

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    std::shared_ptr<sql::ResultSet> res;
    pstmt->setInt(1, static_cast<int>(board_id));

    try {
        res.reset(pstmt->executeQuery());
        if (!res->next()) {
            return OBJECT_NOT_EXIST;
        }

        std::string res_password = res->getString("password");
        return (res_password == password) ? OK : WRONG_PASSWORD;

    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }
}


int Board::AddUserToBoard(const std::size_t &board_id, const std::size_t &player_id, const std::string &password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    int out = CheckPassword(board_id, password);
    if (out != OK) {
        return out;
    }

    std::string query_string = "INSERT INTO active_board(board_id, player_id, reserved_money) VALUES (?, ?, ?)";

    std::shared_ptr<sql::PreparedStatement> pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(board_id));
    pstmt->setInt(2, static_cast<int>(player_id));
    pstmt->setDouble(3, 0.0);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        return OBJECT_ALREADY_EXIST;
    }
    return OK;
}


int Board::RemoveUserFromBoard(const std::size_t &board_id, const std::size_t &player_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::string query_string = "DELETE FROM active_board WHERE board_id = ? AND player_id = ?";
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(board_id));
    pstmt->setInt(2, static_cast<int>(player_id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }
    return OK;
}


int Board::SetReservedMoney(const std::size_t &board_id, const std::size_t &player_id, const double &reserved_money) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    User usr;
    auto user_out = usr.GetUser(player_id);
    if (user_out.status_code != OK) {
        return static_cast<int>(user_out.status_code);
    }
    if (user_out.money < reserved_money) {
        return INSUFFICIENT_FUNDS;
    }

    auto func = [](std::shared_ptr<sql::PreparedStatement> &pstmt, const double &data) {
        pstmt->setDouble(1, data);
    };
    return UpdateField<double>(board_id, player_id, "reserved_money", reserved_money, func);
}


int Board::UpdateUserPosition(const std::size_t &board_id, const std::size_t &player_id, const int &pos) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    auto func = [](std::shared_ptr<sql::PreparedStatement> &pstmt, const int &data) {
        pstmt->setInt(1, data);
    };
    return UpdateField<int>(board_id, player_id, "position", pos, func);
}


template<class T>
int Board::UpdateField(const std::size_t &board_id, const std::size_t &player_id, const std::string &field_name,
                       const T &data, void (*functor)(std::shared_ptr<sql::PreparedStatement>&, const T&)) {
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::string query_string = "UPDATE active_board SET " + field_name + " = ? WHERE board_id = ? AND player_id = ?";
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    functor(pstmt, data);
    pstmt->setInt(2, static_cast<int>(board_id));
    pstmt->setInt(3, static_cast<int>(player_id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }
    return OK;
}

}  // namespace database