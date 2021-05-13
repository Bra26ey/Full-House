#include "User.h"

namespace database {

bool User::IsExist(const std::size_t& id) {
    std::string query_string = "SELECT id FROM user WHERE id = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    if (!conn_.GetConnection().IsOpen()) {
        return false;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int)id);

    try {
        res = pstmt->executeQuery();
        if (!res->next()) {
            delete pstmt;
            delete res;
            return false;
        }

        std::size_t res_id = res->getInt("id");
        if (res_id != id) {
            delete pstmt;
            delete res;
            return false;
        }

    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
        return false;
    }
    delete pstmt;
    delete res;
    return true;
}


bool User::IsExist(const std::string &login) {
    std::string query_string = "SELECT login FROM user WHERE login = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    if (!conn_.GetConnection().IsOpen()) {
        return false;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setString(1, login);

    try {
        res = pstmt->executeQuery();
        if (!res->next()) {
            delete pstmt;
            delete res;
            return false;
        }

        std::string res_login = res->getString("login");
        if (res_login != login) {
            delete pstmt;
            delete res;
            return false;
        }

    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
        return false;
    }
    delete pstmt;
    delete res;
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
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int)id);

    try {
        res = pstmt->executeQuery();
        if (!res->next()) {
            u.status_code = OBJECT_NOT_EXIST;
            delete pstmt;
            delete res;
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
        u.money = (double) res->getDouble("money");
        u.status_code = OK;
    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }
    delete pstmt;
    delete res;
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
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setString(1, login);

    try {
        res = pstmt->executeQuery();
        if (!res->next()) {
            u.status_code = OBJECT_NOT_EXIST;
            delete pstmt;
            delete res;
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
        u.money = (double) res->getDouble("money");
        u.status_code = OK;
    } catch (sql::SQLException &e) {
        delete pstmt;
        delete res;
        u.status_code = OBJECT_NOT_EXIST;
        return u;
    }
    delete pstmt;
    delete res;
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

    sql::PreparedStatement *pstmt;
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setString(1, login);
    pstmt->setString(2, password);
    pstmt->setString(3, username);
    pstmt->setString(4, avatar);
    pstmt->setDouble(5, money);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return std::pair<std::size_t, int> (0, OBJECT_ALREADY_EXIST);
    }

    query_string = "SELECT MAX(id) from user";
    sql::Statement *stmt = conn_.GetConnection().SetQuery(query_string);
    sql::ResultSet *res = stmt->executeQuery(query_string);
    std::size_t last_id = 0;
    if (res->next()) {
        last_id = res->getInt(1);
    }

    delete pstmt;
    delete stmt;
    delete res;
    return std::pair<std::size_t, int> (last_id, OK);
}


int User::UpdateStringField(const std::string &field_name, const std::size_t &id, const std::string &data) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    sql::PreparedStatement *pstmt;
    std::string query_string = "UPDATE user SET " + field_name + " = ? WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setString(1, data);
    pstmt->setInt(2, (int) id);

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

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setDouble(1, money);
    pstmt->setInt(2, (int) id);

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


int User::DeleteUser(const size_t &id) {
    std::lock_guard<std::mutex> lock(conn_.GetMutex());

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM user WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) id);

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

} // namespace database
