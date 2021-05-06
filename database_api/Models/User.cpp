#include "User.h"


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


user_t User::GetUser(const std::size_t& id, bool with_password=false) {
    user_t u;

    std::string query_string = "SELECT * FROM user WHERE id = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_.GetConnection().IsOpen()) {
        u.status_code = DATABASE_NOT_CONNECTED;
        return u;
    }

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


user_t User::GetUser(const std::string & login, bool with_password=false) {
    user_t u;

    std::string query_string = "SELECT * FROM user WHERE login = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_.GetConnection().IsOpen()) {
        u.status_code = DATABASE_NOT_CONNECTED;
        return u;
    }

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


std::pair<std::size_t, int> User::InsertUser(const std::string &login, const std::string &password) {
    conn_.Lock();

    if (login.empty() || password.empty()) {
        conn_.Unlock();
        return std::pair<std::size_t, int> (0, EMPTY_DATA);
    }

    if (IsExist(login)) {
        conn_.Unlock();
        return std::pair<std::size_t, int> (0, OBJECT_ALREADY_EXIST);
    }

    std::string query_string = "INSERT INTO user(login, password, username, avatar, money) VALUES (?, ?, ?, ?, ?)";
    std::string username = login;
    std::string avatar = DEFAULT_PATH_TO_IMAGE;
    double money = 0.0;

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return std::pair<std::size_t, int> (0, DATABASE_NOT_CONNECTED);
    }

    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setString(1, login);
    pstmt->setString(2, password);
    pstmt->setString(3, username);
    pstmt->setString(4, avatar);
    pstmt->setDouble(5, money);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        conn_.Unlock();
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
    conn_.Unlock();

    delete pstmt;
    delete stmt;
    delete res;
    return std::pair<std::size_t, int> (last_id, OK);
}


int User::UpdateStringField(const std::string &field_name, const std::size_t &id, const std::string &data) {
    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

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
    conn_.Lock();

    if (new_login.empty()) {
        conn_.Unlock();
        return EMPTY_DATA;
    }
    conn_.Unlock();
    return UpdateStringField("login", id, new_login);
}


int User::UpdatePassword(const size_t &id, const std::string &new_password) {
    conn_.Lock();

    if (new_password.empty()) {
        conn_.Unlock();
        return EMPTY_DATA;
    }
    conn_.Unlock();
    return UpdateStringField("password", id, new_password);
}


int User::UpdateUsername(const size_t &id, const std::string &new_username) {
    conn_.Lock();

    if (new_username.empty()) {
        conn_.Unlock();
        return EMPTY_DATA;
    }
    conn_.Unlock();
    return UpdateStringField("username", id, new_username);
}


int User::UpdateAvatar(const size_t &id, const std::string &new_avatar) {
    conn_.Lock();

    if (new_avatar.empty()) {
        conn_.Unlock();
        return EMPTY_DATA;
    }
    conn_.Unlock();
    return UpdateStringField("avatar", id, new_avatar);
}


int User::UpdateMoney(const size_t &id, double new_money) {
    conn_.Lock();

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE user SET money = ? WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setDouble(1, new_money);
    pstmt->setInt(2, (int) id);

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


int User::DeleteUser(const size_t &id) {
    conn_.Lock();

    sql::PreparedStatement *pstmt;
    if (!conn_.GetConnection().IsOpen()) {
        conn_.Unlock();
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM user WHERE id = ?";
    pstmt = conn_.GetConnection().PrepareQuery(query_string);
    pstmt->setInt(1, (int) id);

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