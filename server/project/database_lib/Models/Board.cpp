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
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        res = pstmt->executeQuery();
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
        delete pstmt;
        delete res;
        b.status_code = OBJECT_NOT_EXIST;
        return b;
    }
    delete pstmt;
    delete res;
    return b;
}


active_board_t Board::GetActiveBoard(const std::size_t &board_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    active_board_t act_board;

    if (!conn_.GetConnection().IsOpen()) {
        act_board.status_code = DATABASE_NOT_CONNECTED;
        return act_board;
    }

    // std::string query_string = "SELECT * FROM active_board WHERE board_id = ?";
    std::string query_string = "SELECT user.id, user.username, user.avatar, active_board.reserved_money, active_board.position, "
                               "board.button_pos, board.small_blind_pos, board.big_blind_pos, board.small_blind_bet, board.big_blind_bet, "
                               "board.max_size_of_players, board.count_of_player_cards FROM user JOIN active_board ON user.id = "
                               "active_board.player_id JOIN board ON active_board.board_id = board.id WHERE board.id = ?;";
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        res = pstmt->executeQuery();
        player_t cur_player;
        if (res->next()) {
            act_board.board_id = board_id;
            cur_player.id = res->getInt("id");
            cur_player.username = res->getString("username");
            cur_player.avatar = res->getString("avatar");
            cur_player.reserved_money = (double) res->getDouble("reserved_money");
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
            cur_player.reserved_money = (double) res->getDouble("reserved_money");
            cur_player.position = res->getInt("position");
            act_board.players.push_back(cur_player);
        }
        act_board.status_code = OK;
    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
        act_board.status_code = OBJECT_NOT_EXIST;
        return act_board;
    }

    delete pstmt;
    delete res;

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
    sql::PreparedStatement *pstmt;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) active_brd.hand_config.button_pos);
    pstmt->setInt(2, (int) active_brd.hand_config.small_blind_pos);
    pstmt->setInt(3, (int) active_brd.hand_config.big_blind_pos);
    pstmt->setInt(4, (int) active_brd.hand_config.small_blind_bet);
    pstmt->setInt(5, (int) active_brd.hand_config.big_blind_bet);
    pstmt->setInt(6, (int) active_brd.hand_config.max_size_of_players);
    pstmt->setInt(7, (int) active_brd.hand_config.count_of_player_cards);
    pstmt->setInt(8, (int) active_brd.board_id);

    // we admit that the system is not updatable
    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    delete pstmt;
    // update active_board table
    query_string = "UPDATE active_board SET position = ?, reserved_money = ? WHERE board_id = ? AND player_id = ?;";
    for (auto & player : active_brd.players) {
        pstmt = conn_.GetConnection().PrepareQuery(query_string);
        pstmt->setInt(   1, player.position);
        pstmt->setDouble(2, player.reserved_money);
        pstmt->setInt(   3, (int) active_brd.board_id);
        pstmt->setInt(   4, (int) player.id);
        try {
            pstmt->executeUpdate();
        } catch (sql::SQLException &e) {
            delete pstmt;
            return OBJECT_NOT_EXIST;
        }
    }

    delete pstmt;
    return OK;
}

/*
user_t Board::GetUserId(const std::string &login) {
    user_t u;

    if (login.empty()) {
        u.status_code = EMPTY_DATA;
        return u;
    }

    User usr;

    if (!usr.IsExist(login)) {
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }

    u = usr.GetUser(login, false);
    return u;
}*/


std::pair<std::size_t, int> Board::CreateBoard(const std::size_t &user_id, const std::string& password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return std::pair<std::size_t, int> (0, DATABASE_NOT_CONNECTED);
    }

    std::string query_string = "INSERT INTO board(admin_id, password) VALUES (?, ?)";
    sql::PreparedStatement *pstmt;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) user_id);
    pstmt->setString(2, password);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return std::pair<std::size_t, int> (0, OBJECT_NOT_EXIST);
    }
    delete pstmt;

    query_string = "SELECT MAX(id) from board";
    sql::Statement *stmt = conn_.GetConnection().SetQuery(query_string);
    sql::ResultSet *res = stmt->executeQuery(query_string);
    std::size_t last_id = 0;
    if (res->next()) {
        last_id = res->getInt(1);
    }

    delete stmt;
    delete res;
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
        return (int) user_out.status_code;
    }

    std::string query_string = "UPDATE board SET admin_id = ? WHERE id = ?";
    sql::PreparedStatement *pstmt;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) new_admin_id);
    pstmt->setInt(2, (int) board_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            delete pstmt;
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }

    delete pstmt;
    return OK;
}


int Board::DeleteBoard(const std::size_t &board_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    sql::PreparedStatement *pstmt;
    std::string query_string = "DELETE FROM board WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            delete pstmt;
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }

    delete pstmt;
    return OK;
}


int Board::UpdateHandConfiguration(const std::size_t &board_id, const hand_configuration_t &hand_config) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    //board.button_pos, board.small_blind_pos, board.big_blind_pos, board.small_blind_bet, board.big_blind_bet, "
    //                               "board.max_size_of_players, board.count_of_player_cards
    std::string query_string = "UPDATE board SET button_pos = ?, small_blind_pos = ?, big_blind_pos = ?, small_blind_bet = ?, "
                               "big_blind_bet = ?, max_size_of_players = ?, count_of_player_cards = ? WHERE id = ?";
    sql::PreparedStatement *pstmt;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) hand_config.button_pos);
    pstmt->setInt(2, (int) hand_config.small_blind_pos);
    pstmt->setInt(3, (int) hand_config.big_blind_pos);
    pstmt->setInt(4,       hand_config.small_blind_bet);
    pstmt->setInt(5,       hand_config.big_blind_bet);
    pstmt->setInt(6, (int) hand_config.max_size_of_players);
    pstmt->setInt(7, (int) hand_config.count_of_player_cards);
    pstmt->setInt(8, (int) board_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            delete pstmt;
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }

    delete pstmt;
    return OK;
}


int Board::CheckPassword(const std::size_t &board_id, const std::string &password) {
    std::string query_string = "SELECT password FROM board WHERE id = ?";

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        res = pstmt->executeQuery();
        if (!res->next()) {
            delete pstmt;
            delete res;
            return OBJECT_NOT_EXIST;
        }

        std::string res_password = res->getString("password");
        delete pstmt;
        delete res;
        return (res_password == password) ? OK : WRONG_PASSWORD;

    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
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

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) player_id);
    pstmt->setDouble(3, 0.0);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_ALREADY_EXIST;
    }
    delete pstmt;
    return OK;
}


int Board::RemoveUserFromBoard(const std::size_t &board_id, const std::size_t &player_id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    sql::PreparedStatement *pstmt;
    std::string query_string = "DELETE FROM active_board WHERE board_id = ? AND player_id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) player_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            delete pstmt;
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    delete pstmt;
    return OK;
}


int Board::SetReservedMoney(const std::size_t &board_id, const std::size_t &player_id, const double &reserved_money) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    User usr;
    auto user_out = usr.GetUser(player_id);
    if (user_out.status_code != OK) {
        return (int) user_out.status_code;
    }
    if (user_out.money < reserved_money) {
        return INSUFFICIENT_FUNDS;
    }

    auto func = [](sql::PreparedStatement *&pstmt, const double &data) {
        pstmt->setDouble(1, data);
    };
    return UpdateField<double>(board_id, player_id, "reserved_money", reserved_money, func);
}


int Board::UpdateUserPosition(const std::size_t &board_id, const std::size_t &player_id, const int &pos) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    auto func = [](sql::PreparedStatement *&pstmt, const int &data) {
        pstmt->setInt(1, data);
    };
    return UpdateField<int>(board_id, player_id, "position", pos, func);
}


template<class T>
int Board::UpdateField(const std::size_t &board_id, const std::size_t &player_id, const std::string &field_name,
                       const T &data, void (*functor)(sql::PreparedStatement *&, const T&)) {
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    sql::PreparedStatement *pstmt;
    std::string query_string = "UPDATE active_board SET " + field_name + " = ? WHERE board_id = ? AND player_id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    functor(pstmt, data);
    pstmt->setInt(2, (int) board_id);
    pstmt->setInt(3, (int) player_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            delete pstmt;
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    delete pstmt;
    return OK;
}

}  // namespace database