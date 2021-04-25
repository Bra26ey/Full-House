#include "Board.h"


board_t Board::GetBoard(const std::size_t &board_id) {
    board_t b;

    std::string query_string = "SELECT * FROM board WHERE board_id = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_->IsOpen()) {
        b.status_code = DATABASE_NOT_CONNECTED;
        return b;
    }

    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);

    try {
        res = pstmt->executeQuery();
        std::pair<std::size_t, double> pr(0, 0.0);
        if (res->next()) {
            b.board_id = res->getInt("board_id");
            pr.first = res->getInt("player_id");
            pr.second = (double) res->getDouble("reserved_money");
            b.users.push_back(pr);
        }
        while (res->next()) {
            pr.first = res->getInt("player_id");
            pr.second = (double) res->getDouble("reserved_money");
            b.users.push_back(pr);
        }
        b.status_code = OK;
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


user_t Board::GetUserId(const std::string &login) {
    user_t u;

    if (login.empty()) {
        u.status_code = EMPTY_DATA;
        return u;
    }

    User usr = User(conn_);

    if (!usr.IsExist(login)) {
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }

    u = usr.GetUser(login, false);
    return u;
}


std::pair<std::size_t, int> Board::CreateBoard(const std::string &login) {
    user_t u = GetUserId(login);
    if (u.status_code != OK) {
        return std::pair<std::size_t, int> (0, u.status_code);
    }
    std::string query_string = "INSERT INTO board(player_id, reserved_money) VALUES (?, ?)";
    // insert into board (board_id, player_id, reserved_money) values ((SELECT MAX(b.board_id) + 1 from board as b), 1, 25);

    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return std::pair<std::size_t, int> (0, DATABASE_NOT_CONNECTED);
    }

    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setInt(1, (int) u.id);
    pstmt->setDouble(2, 0.0);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return std::pair<std::size_t, int> (0, OBJECT_ALREADY_EXIST);
    }
    delete pstmt;

    query_string = "SELECT MAX(board_id) from board";
    sql::Statement *stmt = conn_->SetQuery(query_string);
    sql::ResultSet *res = stmt->executeQuery(query_string);
    std::size_t last_id = 0;
    if (res->next()) {
        last_id = res->getInt(1);
    }

    delete stmt;
    delete res;
    return std::pair<std::size_t, int> (last_id, OK);
}


int Board::DeleteBoard(const std::size_t &board_id) {
    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM board WHERE board_id = ?";
    pstmt = conn_->PrepareQuery(query_string);
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


int Board::AddUserToBoard(const std::size_t &board_id, const std::string &login) {
    user_t u = GetUserId(login);
    if (u.status_code != OK) {
        return (int) u.status_code;
    }

    std::string query_string = "INSERT INTO board(board_id, player_id, reserved_money) VALUES (?, ?, ?)";

    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) u.id);
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


int Board::RemoveUserFromBoard(const std::size_t &board_id, const std::string &login) {
    user_t u = GetUserId(login);
    if (u.status_code != OK) {
        return (int) u.status_code;
    }

    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM board WHERE board_id = ? AND player_id = ?";
    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setInt(1, (int) board_id);
    pstmt->setInt(2, (int) u.id);

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


int Board::SetReservedMoney(const std::size_t &board_id, const std::string &login, const double &reserved_money) {
    user_t u = GetUserId(login);
    if (u.status_code != OK) {
        return (int) u.status_code;
    }

    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE board SET reserved_money = ? WHERE board_id = ? AND player_id = ?";
    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setDouble(1, reserved_money);
    pstmt->setInt(2, (int) board_id);
    pstmt->setInt(3, (int) u.id);

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
