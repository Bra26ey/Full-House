#include "Board.h"


board_t Board::GetBoard(const std::size_t &board_id, bool with_password = false) {
    conn_.Lock();
    board_t b;

    std::string query_string = "SELECT * FROM board WHERE id = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        b.status_code = DATABASE_NOT_CONNECTED;
        return b;
    }

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
        conn_.Unlock();
        delete pstmt;
        delete res;
        b.status_code = OBJECT_NOT_EXIST;
        return b;
    }
    conn_.Unlock();
    delete pstmt;
    delete res;
    return b;
}


active_board_t Board::GetActiveBoard(const std::size_t &board_id) {
    conn_.Lock();
    active_board_t act_board;
    std::string query_string = "SELECT * FROM active_board WHERE board_id = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        act_board.status_code = DATABASE_NOT_CONNECTED;
        return act_board;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        res = pstmt->executeQuery();
        std::pair<std::size_t, double> pr(0, 0.0);
        if (res->next()) {
            act_board.board_id = res->getInt("board_id");
            pr.first = res->getInt("player_id");
            pr.second = (double) res->getDouble("reserved_money");
            act_board.users.push_back(pr);
        }
        while (res->next()) {
            pr.first = res->getInt("player_id");
            pr.second = (double) res->getDouble("reserved_money");
            act_board.users.push_back(pr);
        }
        act_board.status_code = OK;
    } catch (sql::SQLException &e) {
        conn_.Unlock();
        delete pstmt;
        delete res;
        act_board.status_code = OBJECT_NOT_EXIST;
        return act_board;
    }
    conn_.Unlock();
    delete pstmt;
    delete res;

    return act_board;
}


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
}


std::pair<std::size_t, int> Board::CreateBoard(const std::size_t &user_id, const std::string& password) {
    conn_.Lock();
    std::string query_string = "INSERT INTO board(admin_id, password) VALUES (?, ?)";

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return std::pair<std::size_t, int> (0, DATABASE_NOT_CONNECTED);
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) user_id);
    pstmt->setString(2, password);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        conn_.Unlock();
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
    conn_.Unlock();

    delete stmt;
    delete res;
    if (last_id != 0) {
        return std::pair<std::size_t, int>(last_id, OK);
    }
    else {
        return std::pair<std::size_t, int>(last_id, BAD_QUERY);
    }
}


int Board::DeleteBoard(const std::size_t &board_id) {
    conn_.Lock();
    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM board WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            conn_.Unlock();
            delete pstmt;
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        conn_.Unlock();
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    conn_.Unlock();

    delete pstmt;
    return OK;
}


int Board::CheckPassword(const std::size_t &board_id, const std::string &password) {
    std::string query_string = "SELECT password FROM board WHERE id = ?";
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

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
    conn_.Lock();
    int out = CheckPassword(board_id, password);
    if (out != OK) {
        conn_.Unlock();
        return out;
    }

    std::string query_string = "INSERT INTO active_board(board_id, player_id, reserved_money) VALUES (?, ?, ?)";

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) player_id);
    pstmt->setDouble(3, 0.0);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        conn_.Unlock();
        delete pstmt;
        return OBJECT_ALREADY_EXIST;
    }
    conn_.Unlock();
    delete pstmt;
    return OK;
}


int Board::RemoveUserFromBoard(const std::size_t &board_id, const std::size_t &player_id) {
    conn_.Lock();
    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM active_board WHERE board_id = ? AND player_id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) player_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            conn_.Unlock();
            delete pstmt;
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        conn_.Unlock();
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    conn_.Unlock();
    delete pstmt;
    return OK;
}


int Board::SetReservedMoney(const std::size_t &board_id, const std::size_t &player_id, const double &reserved_money) {
    conn_.Lock();
    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE active_board SET reserved_money = ? WHERE board_id = ? AND player_id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setDouble(1, reserved_money);
    pstmt->setInt(2, (int) board_id);
    pstmt->setInt(3, (int) player_id);

    try {
        if (pstmt->executeUpdate() == 0) {
            conn_.Unlock();
            delete pstmt;
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        conn_.Unlock();
        delete pstmt;
        return OBJECT_NOT_EXIST;
    }
    conn_.Unlock();

    delete pstmt;
    return OK;
}
