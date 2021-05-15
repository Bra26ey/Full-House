#pragma once

namespace database {

constexpr int OK = 1;
constexpr int DATABASE_NOT_CONNECTED = 11;
constexpr int OBJECT_ALREADY_EXIST = 21;
constexpr int OBJECT_NOT_EXIST = 22;
constexpr int OBJECT_NOT_UPDATED = 23;
constexpr int EMPTY_DATA = 24;
constexpr int BAD_QUERY = 25;
constexpr int WRONG_PASSWORD = 26;
constexpr int INSUFFICIENT_FUNDS = 27;
constexpr int FILE_NOT_OPENED = 31;
constexpr int EMPTY_FIELD = 32;

#define FILE_DIR "/project/database_lib/FileHandler/"
#define FILE_NOT_OPENED_MSG "File not opened"
#define DEFAULT_PATH_TO_IMAGE "default_path_to_image"

constexpr int POOL_SIZE = 10;
constexpr int MAX_POOL_SIZE = 40;

constexpr int FREE_CONN = 0;
constexpr int BUSY_CONN = 1;

}  // namespace database
