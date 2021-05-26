#include "FileHandler.h"

namespace database {

FileHandler::FileHandler() {
    absolute_path_ = std::filesystem::current_path().string();
    std::size_t last_slash_pos = absolute_path_.find_last_of("/");
    absolute_path_ = absolute_path_.erase(last_slash_pos);
}


std::map<std::string, std::string> FileHandler::Parse(const std::string& filename) {
    std::map<std::string, std::string> parser;
    std::ifstream cfile(absolute_path_ + FILE_DIR + filename);

    if (!cfile.is_open()) {
        parser["status"] = FILE_NOT_OPENED_MSG;
        return parser;
    }

    std::string line;
    while (getline(cfile, line)) {
        if (line[0] == '#' || line.empty()) {
            continue;
        }

        auto delimiter_pos = line.find("=");
        auto name = line.substr(0, delimiter_pos);
        auto value = line.substr(delimiter_pos + 1);

        parser[name] = value;
    }
    cfile.close();

    parser["status"] = "OK";
    return parser;
}


config_t FileHandler::ParseConfig(const std::string& filename) {
    config_t cnfg;
    std::map<std::string, std::string> parser = Parse(filename);
    if (parser["status"] != "OK") {
        cnfg.status_code = FILE_NOT_OPENED;
        return cnfg;
    }

    for (auto & elem : parser) {
        Fill(elem.first, elem.second, cnfg);
    }

    cnfg.status_code = EmptyField(cnfg) ? EMPTY_FIELD : OK;
    return cnfg;
}


db_schemas_t FileHandler::ParseDbSchema(const std::string& filename) {
    db_schemas_t dbscm;
    std::map<std::string, std::string> parser = Parse(filename);
    if (parser["status"] != "OK") {
        dbscm.status_code = FILE_NOT_OPENED;
        return dbscm;
    }

    for (auto & elem : parser) {
        Fill(elem.first, elem.second, dbscm);
    }

    dbscm.status_code = EmptyField(dbscm) ? EMPTY_FIELD : OK;
    return dbscm;
}


void FileHandler::Fill(const std::string &name, const std::string &source, config_t& cnfg) {
    if (name == "url") {
        cnfg.url = source;
    }
    else if (name == "user") {
        cnfg.user = source;
    }
    else if (name == "password") {
        cnfg.password = source;
    }
    else if (name == "database") {
        cnfg.database = source;
    }
}


void FileHandler::Fill(const std::string &name, const std::string &source, db_schemas_t &dbscm) {
    if (name == "USER_SCHEMA") {
        dbscm.user = source;
    }
    else if (name == "BOARD_SCHEMA") {
        dbscm.board = source;
    }
    else if (name == "ACTIVE_BOARD_SCHEMA") {
        dbscm.active_board = source;
    }
}


bool FileHandler::EmptyField(const config_t &cnfg) {
    return cnfg.url.empty() || cnfg.user.empty() || cnfg.password.empty() || cnfg.database.empty();
}

bool FileHandler::EmptyField(const db_schemas_t &dbscm) {
    return dbscm.user.empty() || dbscm.board.empty() || dbscm.active_board.empty();
}

}  // namespace database
