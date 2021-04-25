#include "User.h"


bool User::IsExist(const std::string &login) {
    if (login.empty()) {
        return false;
    }

    std::string query_string = "SELECT login FROM user WHERE login = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    if (!conn_->IsOpen()) {
        return false;
    }

    pstmt = conn_->PrepareQuery(query_string);
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


user_t User::GetUser(const std::string &login, bool with_password=false) {
    user_t u;
    if (login.empty()) {
        u.status_code = EMPTY_DATA;
        return u;
    }

    std::string query_string = "SELECT * FROM user WHERE login = ?";

    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    if (!conn_->IsOpen()) {
        u.status_code = DATABASE_NOT_CONNECTED;
        return u;
    }

    pstmt = conn_->PrepareQuery(query_string);
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


int User::InsertUser(const std::string &login, const std::string &password) {
    if (login.empty() || password.empty()) {
        return EMPTY_DATA;
    }

    if (IsExist(login)) {
        return OBJECT_ALREADY_EXIST;
    }

    std::string query_string = "INSERT INTO user(login, password, username, avatar, money) VALUES (?, ?, ?, ?, ?)";
    std::string username = login;
    std::string avatar = DEFAULT_PATH_TO_IMAGE;
    double money = 0.0;

    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setString(1, login);
    pstmt->setString(2, password);
    pstmt->setString(3, username);
    pstmt->setString(4, avatar);
    pstmt->setDouble(5, money);

    try {
        pstmt->executeUpdate();
    } catch (sql::SQLException &e) {
        delete pstmt;
        return OBJECT_ALREADY_EXIST;
    }
    delete pstmt;
    return OK;
}


int User::UpdateStringField(const std::string &field_name, const std::string &login, const std::string &data) {
    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE user SET " + field_name + " = ? WHERE login = ?";
    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setString(1, data);
    pstmt->setString(2, login);

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


int User::UpdateLogin(const std::string &login, const std::string &new_login) {
    if (new_login.empty() || login.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("login", login, new_login);
}


int User::UpdatePassword(const std::string &login, const std::string &new_password) {
    if (new_password.empty() || login.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("password", login, new_password);
}


int User::UpdateUsername(const std::string &login, const std::string &new_username) {
    if (new_username.empty() || login.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("username", login, new_username);
}


int User::UpdateAvatar(const std::string &login, const std::string &new_avatar) {
    if (new_avatar.empty() || login.empty()) {
        return EMPTY_DATA;
    }
    return UpdateStringField("avatar", login, new_avatar);
}


int User::UpdateMoney(const std::string &login, double new_money) {
    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "UPDATE user SET money = ? WHERE login = ?";
    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setDouble(1, new_money);
    pstmt->setString(2, login);

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


int User::DeleteUser(const std::string &login) {
    sql::PreparedStatement *pstmt;
    if (!conn_->IsOpen()) {
        return DATABASE_NOT_CONNECTED;
    }

    std::string query_string = "DELETE FROM user WHERE login = ?";
    pstmt = conn_->PrepareQuery(query_string);
    pstmt->setString(1, login);

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