#include "gametalker.h"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "convert.h"

#include "msgmaker.h"

using boost::asio::async_read;
using boost::asio::async_write;

constexpr uint8_t USERS_MAX = 6;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace network {

GameTalker::GameTalker(io_context &context, database::Board &board_db,
                       database::User &user_db, std::shared_ptr<User> &user)
                       : is_remove(false),
                         is_gaming(false),
                         context_(context),
                         board_db_(board_db),
                         user_db_(user_db),
                         handprocess_(logic::DECK_SIZE),
                         is_deleting_(false) {
    const pt::ptree &parametrs = user->last_msg.get_child("parametrs");
    auto password = parametrs.get<std::string>("password");

    auto answer = board_db_.CreateBoard(user->id, password);
    if (answer.second != database::OK) {
        is_remove.store(true);
        return;
    }

    id = answer.first;
    database::hand_configuration_t hand_config;
    hand_config.button_pos = 0;
    hand_config.small_blind_pos = 0;
    hand_config.big_blind_pos = 1;
    hand_config.small_blind_bet = 1;
    hand_config.big_blind_bet = 2;
    hand_config.max_size_of_players = USERS_MAX;
    hand_config.count_of_player_cards = 2;
    board_db_.UpdateHandConfiguration(id, hand_config);
    admin_id_.store(user->id);

    auto code = board_db_.AddUserToBoard(id, user->id, password);
    if (code != database::OK) {
        CreatingFailed(user);
        is_remove.store(true);
        return;
    }

    auto position = positions_.Insert(user->id);
    if (position == TPOS_ERROR) {
        CreatingFailed(user);
        is_remove.store(true);
        return;
    }

    code = board_db_.UpdateUserPosition(static_cast<size_t>(id),
                                        static_cast<size_t>(user->id), 
                                        static_cast<int>(position));
    if (code != database::OK && code != database::OBJECT_NOT_UPDATED) {
        CreatingFailed(user);
        is_remove.store(true);
        return;
    }

    code = board_db_.SetReservedMoney(id, user->id, user->money);
    if (code != database::OK && code != database::OBJECT_NOT_UPDATED) {
        CreatingFailed(user);
        is_remove.store(true);
        return;
    }

    users_.push_back(user);

    user->is_gaming.store(true);
    user->is_talking.store(false);
    user->room_id = id;

    auto hand = convert(board_db_.GetActiveBoard(id));
    handprocess_.Init(hand);

    read_until(user->socket, user->read_buffer, std::string(MSG_END));
    pt::read_json(user->in, user->last_msg);

    user->out << MsgServer::CreateRoomDone(id, password);
    async_write(user->socket, user->write_buffer, [this, user](bs::error_code error, size_t len) {
        if (!error) {
            // boost::asio::post(context_, boost::bind(&UserTalker::OnHandleRequest, this));
            OnHandleUserRequest(user);
        } else {
            // boost::asio::post(context_, boost::bind(&UserTalker::Disconnect, this));
            CreatingFailed(user);
            is_remove.store(true);
        }
    });
}

GameTalker::~GameTalker() {
    board_db_.DeleteBoard(id);
}

void GameTalker::Start() {
    BOOST_LOG_TRIVIAL(info) << "GameRoom Start Game";
    boost::asio::post(context_, boost::bind(&GameTalker::HandleGameProcess, this));
}

void GameTalker::SendAnswer(std::shared_ptr<User> user) {
    async_write(user->socket, user->write_buffer, [this, user](bs::error_code error, size_t len) {
        if (!error) {
            OnHandleUserRequest(user);
        } else {
            // add clever handle
            OnHandleUserRequest(user);
        }
    });
}

void GameTalker::HandleAdminRequest(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << "GameRoom handleing admin request: ";

    std::string command = user->last_msg.get<std::string>("command");

    std::lock_guard<std::mutex> lock(users_mutex_);

    if (command == "start-game") {
        if (users_.size() > 1) {
            is_gaming.store(true);
            user->out << MsgServer::StartGameDone();
            boost::asio::post(context_, boost::bind(&GameTalker::Start, this));
        } else {
            user->out << MsgServer::StartGameFailed();
        }
        boost::asio::post(context_, boost::bind(&GameTalker::SendAnswer, this, user));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

void GameTalker::OnHandleUserRequest(std::shared_ptr<User> user) {
    async_read_until(user->socket, user->read_buffer, std::string(MSG_END), [this, user](bs::error_code error, size_t len) {
        if (!error) {
            // boost::bind(&GameTalker::HandleUserRequest, this, user);
            HandleUserRequest(user);
        } else {
            // add timer
            OnHandleUserRequest(user);
            // handle leaving
        }
    });
}

void GameTalker::HandleGameStatus(std::shared_ptr<User> user) {
    auto admin_pos = positions_.GetPosition(admin_id_.load());
    user->out << MsgServer::GameStatus(handprocess_.GetGameStatus(), admin_pos);
    boost::asio::post(context_, boost::bind(&GameTalker::SendAnswer, this, user));
    return;
}

void GameTalker::HandleUserRequest(std::shared_ptr<User> user) {
    if (is_remove) {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    pt::read_json(user->in, user->last_msg);
    std::string command_type = user->last_msg.get<std::string>("command-type");

    if (command_type == "ping") {
         boost::asio::post(context_, boost::bind(&GameTalker::HandleGameStatus, this, user));
         return;
    }

    if (is_gaming) {
        if (command_type == "game") {
            boost::asio::post(context_, boost::bind(&GameTalker::HandleGameRequest, this, user));
        } else {
            boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
        }
        return;
    }

    if (command_type == "room-admin") {
        if (user->id == admin_id_.load()) {
            boost::asio::post(context_, boost::bind(&GameTalker::HandleAdminRequest, this, user));
        } else {
            boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
        }
        return;
    }

    if (command_type == "room-basic") {  // TODO(ANDY) add pos-ty to leave while gaming
        boost::asio::post(context_, boost::bind(&GameTalker::HandleLeaving, this, user));
        return;
    }

    boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
}

void GameTalker::CreatingFailed(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << "creating game failed";
    user->room_id = __UINT64_MAX__;
    read_until(user->socket, user->read_buffer, std::string(MSG_END));
    pt::read_json(user->in, user->last_msg);
    user->out << MsgServer::CreateRoomFailed();
    write(user->socket, user->write_buffer);
    user->is_talking.store(false);
}

void GameTalker::JoinPlayerFailed(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << "joining game failed. room-id: " << id;
    read_until(user->socket, user->read_buffer, std::string(MSG_END));
    pt::read_json(user->in, user->last_msg);
    user->out << MsgServer::JoinRoomFailed(user->room_id);
    write(user->socket, user->write_buffer);
    user->room_id = __UINT64_MAX__;
    user->is_talking.store(false);
}

int GameTalker::JoinPlayer(std::shared_ptr<User> &user) {
    if (is_gaming || is_remove) {
        JoinPlayerFailed(user);
        return -1;
    }

    std::lock_guard<std::mutex> lock(users_mutex_);

    if (users_.size() >= USERS_MAX) {
        JoinPlayerFailed(user);
        return -1;
    }

    const pt::ptree &parametrs = user->last_msg.get_child("parametrs");
    auto password = parametrs.get<std::string>("password");

    auto code = board_db_.AddUserToBoard(id, user->id, password);
    if (code != database::OK) {
        JoinPlayerFailed(user);
        return -1;
    }

    auto position = positions_.Insert(user->id);
    if (position == TPOS_ERROR) {
        board_db_.RemoveUserFromBoard(id, user->id);
        JoinPlayerFailed(user);
        return -1;
    }

    code = board_db_.UpdateUserPosition(static_cast<size_t>(id),
                                        static_cast<size_t>(user->id), 
                                        static_cast<int>(position));

    if (code != database::OK && code != database::OBJECT_NOT_UPDATED) {
        board_db_.RemoveUserFromBoard(id, user->id);
        JoinPlayerFailed(user);
        return -1;
    }

    code = board_db_.SetReservedMoney(id, user->id, user->money);
    if (code != database::OK && code != database::OBJECT_NOT_UPDATED) {
        board_db_.RemoveUserFromBoard(id, user->id);
        JoinPlayerFailed(user);
        return -1;
    }

    users_.push_back(user);

    user->is_gaming.store(true);
    user->is_talking.store(false);
    user->room_id = id;

    auto hand_config = convert(board_db_.GetActiveBoard(id));
    handprocess_.Init(hand_config);

    read_until(user->socket, user->read_buffer, std::string(MSG_END));
    pt::read_json(user->in, user->last_msg);

    user->out << MsgServer::JoinRoomDone(id, position);
    write(user->socket, user->write_buffer);

    boost::asio::post(context_, boost::bind(&GameTalker::OnHandleUserRequest, this, user));
    return 0;
}

void GameTalker::HandleError(std::shared_ptr<User> user) {
    user->out << MsgServer::Error();
    boost::asio::post(context_, boost::bind(&GameTalker::SendAnswer, this, user));
    BOOST_LOG_TRIVIAL(info) << user->name << "'s request is unknown";
}

void GameTalker::Delete() {
    is_remove.store(true);
    BOOST_LOG_TRIVIAL(info) << "deleting room. room-id: " << id;
}

void GameTalker::HandleLeaving(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << user->name << " leave room";
    users_mutex_.lock();
    for (size_t i = 0; i < users_.size(); ++i) {
        if (users_[i]->name == user->name) {
            users_.erase(users_.begin() + i);
            break;
        }
    }
    
    if (user->id == admin_id_.load() && users_.size() > 0) {
        admin_id_.store(users_[0]->id);
        board_db_.UpdateBoardAdmin(id, admin_id_.load());  // TODO return control
    }
    users_mutex_.unlock();
    positions_.Delete(user->id);

    board_db_.RemoveUserFromBoard(id, user->id);

    auto hand_config = convert(board_db_.GetActiveBoard(id));
    handprocess_.Init(hand_config);

    user->out << MsgServer::LeaveRoomDone();

    write(user->socket, user->write_buffer);  // ATTENTION!

    user->is_gaming.store(false);
    user->room_id = __UINT64_MAX__;

    std::lock_guard<std::mutex> lock(users_mutex_);
    if (users_.size() == 0) {
        boost::asio::post(context_, boost::bind(&GameTalker::Delete, this));
    }
}

void GameTalker::HandleGameRequest(std::shared_ptr<User> user) {
    BOOST_LOG_TRIVIAL(info) << user->name << " send game-request";
    auto command = user->last_msg.get<std::string>("command");

    bool flag = command == "action" && handprocess_.current_player_pos.load() == positions_.GetPosition(user->id);
    if (flag) {
        auto parametrs = user->last_msg.get_child("parametrs");
        auto action = parametrs.get<std::string>("action-type");

        auto sum = (action == "raise") ? parametrs.get<int>("sum") : 0;
        handprocess_.command_queue.push({ positions_.GetPosition(user->id), action, sum });
    }

    if (command != "status-request" && command != "action") {
        boost::asio::post(context_, boost::bind(&GameTalker::HandleError, this, user));
        return;
    }

    auto admin_pos = positions_.GetPosition(admin_id_.load());
    user->out << MsgServer::GameStatus(handprocess_.GetGameStatus(), admin_pos);
    boost::asio::post(context_, boost::bind(&GameTalker::SendAnswer, this, user));
}

void GameTalker::UpdateTableDatabase() {
    users_mutex_.lock();
    auto hand_config = convert(handprocess_.hand_config);
    board_db_.UpdateHandConfiguration(id, hand_config);
    for (auto &it : handprocess_.hand_config.players) {
        auto user = std::find_if(users_.begin(), users_.end(),
                                 [it](std::shared_ptr<User> &user) { return it->id == user->id; });
        user->get()->money = it->money;
        user_db_.UpdateMoney(it->id, it->money);
    }
    users_mutex_.unlock();
}

void GameTalker::InitReservedMoney() {
    for (auto &it : users_) {
        board_db_.SetReservedMoney(id, it->id, it->money);
    }
}

void GameTalker::HandleGameProcess() {
    reenter(this) {
        while (true) {
            yield {
                InitReservedMoney();
                auto hand_config = convert(board_db_.GetActiveBoard(id));
                handprocess_.Init(hand_config);
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.DealCards();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.Preflop();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.Flop();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.Turn();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.River();
                boost::asio::post(boost::bind(&GameTalker::HandleGameProcess, this));
            }
            yield {
                handprocess_.PotDistribution();
                UpdateTableDatabase();
                is_gaming.store(false);
            }
        }
    }
}

}  // namespace network
