#include "User.h"

namespace database {

bool User::IsExist(const std::size_t& id) {
    std::string query_string = "SELECT id FROM user WHERE id = ?";

    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;

    if (!conn_.GetConnection().IsOpen()) {
        return false;
    }

    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(id));

    try {
        res.reset(pstmt->executeQuery());
        if (!res->next()) {
            return false;
        }

        std::size_t res_id = res->getInt("id");
        if (res_id != id) {
            return false;
        }

    } catch (sql::SQLException &e) {
        return false;
    }

    return true;
}


bool User::IsExist(const std::string &login) {
    std::string query_string = "SELECT login FROM user WHERE login = ?";

    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;

    if (!conn_.GetConnection().IsOpen()) {
        return false;
    }

    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setString(1, login);

    try {
        res.reset(pstmt->executeQuery());
        if (!res->next()) {
            return false;
        }

        std::string res_login = res->getString("login");
        if (res_login != login) {
            return false;
        }

    } catch (sql::SQLException &e) {
        return false;
    }

    return true;
}


user_t User::GetUser(const std::size_t& id, bool with_password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    user_t u;
    if (!conn_.GetConnection().IsOpen()) {
        u.status_code = DATABASE_NOT_CONNECTED;
        return u;
    }

    std::string query_string = "SELECT * FROM user WHERE id = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(id));

    try {
        res.reset(pstmt->executeQuery());
        if (!res->next()) {
            u.status_code = OBJECT_NOT_EXIST;
            return u;
        }

        u.id = res->getInt(1);
        u.login = res->getString("login");
        if (with_password) {
            u.password = res->getString("password");
        } else {
            u.password = "";
        }
        u.username = res->getString("username");
        u.avatar = res->getString("avatar");
        u.money = static_cast<double>(res->getDouble("money"));
        u.status_code = OK;
    } catch (sql::SQLException &e) {
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }

    return u;
}


user_t User::GetUser(const std::string & login, bool with_password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    user_t u;
    if (!conn_.GetConnection().IsOpen()) {
        u.status_code = DATABASE_NOT_CONNECTED;
        return u;
    }

    std::string query_string = "SELECT * FROM user WHERE login = ?";
    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::shared_ptr<sql::ResultSet> res;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setString(1, login);

    try {
        res.reset(pstmt->executeQuery());
        if (!res->next()) {
            u.status_code = OBJECT_NOT_EXIST;
            return u;
        }

        u.id = res->getInt(1);
        u.login = res->getString("login");
        if (with_password) {
            u.password = res->getString("password");
        } else {
            u.password = "";
        }
        u.username = res->getString("username");
        u.avatar = res->getString("avatar");
        u.money = static_cast<double>(res->getDouble("money"));
        u.status_code = OK;
    } catch (sql::SQLException &e) {
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }
    return u;
}


std::pair<std::size_t, int> User::RegUser(const std::string & login, const std::string & password) {
    return InsertUser(login, password);
}


std::pair<user_t , int> User::AuthUser(const std::string &login, const std::string &password) {
    user_t usr = GetUser(login, true);
    if (usr.status_code != OK) {
        return std::pair<user_t, int> (usr, usr.status_code);
    }

    int answer = (usr.login == login && usr.password == password) ? OK : WRONG_PASSWORD;
    return std::pair<user_t, int> (usr, answer);
}


std::pair<std::size_t, int> User::InsertUser(const std::string &login, const std::string &password) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (login.empty() || password.empty()) {
        return std::pair<std::size_t, int> (0, EMPTY_DATA);
    }

    if (IsExist(login)) {
        return std::pair<std::size_t, int> (0, OBJECT_ALREADY_EXIST);
    }

    std::string query_string = "INSERT INTO user(login, password, username, avatar, money) VALUES (?, ?, ?, ?, ?)";
    std::string username = login;
    std::string avatar = DEFAULT_PATH_TO_IMAGE;
    double money = 0.0;

    std::shared_ptr<sql::PreparedStatement> pstmt;
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setString(1, login);
    pstmt->setString(2, password);
    pstmt->setString(3, username);
    pstmt->setString(4, avatar);
    pstmt->setDouble(5, money);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        return std::pair<std::size_t, int> (0, OBJECT_ALREADY_EXIST);
    }

    query_string = "SELECT MAX(id) from user";
    std::shared_ptr<sql::Statement> stmt;
    stmt.reset(conn_.GetConnection().SetQuery(query_string));
    std::shared_ptr<sql::ResultSet> res;
    res.reset(stmt->executeQuery(query_string));
    std::size_t last_id = 0;
    if (res->next()) {
        last_id = res->getInt(1);
    }

    return std::pair<std::size_t, int> (last_id, OK);
}


int User::UpdateStringField(const std::string &field_name, const std::size_t &id, const std::string &data) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::shared_ptr<sql::PreparedStatement> pstmt;
    std::string query_string = "UPDATE user SET " + field_name + " = ? WHERE id = ?";
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setString(1, data);
    pstmt->setInt(2, static_cast<int>(id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}


int User::UpdateLogin(const std::size_t &id, const std::string &new_login) {
    if (new_login.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("login", id, new_login);
}


int User::UpdatePassword(const size_t &id, const std::string &new_password) {
    if (new_password.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("password", id, new_password);
}


int User::UpdateUsername(const size_t &id, const std::string &new_username) {
    if (new_username.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("username", id, new_username);
}


int User::UpdateAvatar(const size_t &id, const std::string &new_avatar) {
    if (new_avatar.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("avatar", id, new_avatar);
}


int User::UpdateMoney(const std::size_t &id, const double &new_money) {
    std::string query_string = "UPDATE user SET money = ? WHERE id = ?";
    return UpdateMoney(id, new_money, query_string);
}


int User::UpdateMoneyByDelta(const std::size_t &id, const double &delta_money) {
    std::string query_string = "UPDATE user SET money = money + ? WHERE id = ?";
    return UpdateMoney(id, delta_money, query_string);
}


int User::UpdateMoney(const size_t &id, const double& money, const std::string& query_string) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    std::shared_ptr<sql::PreparedStatement> pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setDouble(1, money);
    pstmt->setInt(2, static_cast<int>(id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_UPDATED;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}


int User::DeleteUser(const size_t &id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    std::shared_ptr<sql::PreparedStatement> pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM user WHERE id = ?";
    pstmt.reset(conn_.GetConnection().PrepareQuery(query_string));
    pstmt->setInt(1, static_cast<int>(id));

    try {
        if (pstmt->executeUpdate() == 0) {
            return OBJECT_NOT_EXIST;
        }
    } catch (sql::SQLException &e) {
        return OBJECT_NOT_EXIST;
    }

    return OK;
}

} // namespace database
