#include "resolver.h"

#include <algorithm>
#include <QDebug>

#include "client_impl.h"

using namespace screens;

constexpr uint8_t WINNER_NOT_DEFINDED = 10;
constexpr uint8_t MAX_PLAYERS = 6;
constexpr uint8_t MAX_CARDS = 2;

void Resolver::ParseAnswer(pt::ptree const &answer) {
    auto command_type = answer.get<std::string>("command-type");

    if (command_type == "ping") {
        return;
    }

    if (command_type == "error") {
        return;
    }

    if (command_type == "basic-answer") {
        BaseAnswer(answer);
        return;
    }

    if (command_type == "room-basic-answer") {
        RoomBasicAnswer(answer);
        return;
    }

    if (command_type == "room-admin-answer") {
        RoomAdminAnswer(answer);
        return;
    }

    if (command_type == "game-answer") {
        GameAnswer(answer);
        return;
    }
}

void Resolver::BaseAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");


    if (command == "registration") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            QMessageBox msgBox;
            msgBox.setText("Registration succesfully done");
            msgBox.setWindowTitle("Success");
            msgBox.exec();
            back();
        } else {
            QMessageBox msgBox;
            msgBox.setText("Ты инвалид");
            msgBox.setWindowTitle("Ошибка регистрация");
            msgBox.exec();
        }
        return;
    }

    if (command == "autorisation") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            qDebug("Yo");
            navigateTo(MAIN_TAG);
        } else {
            QMessageBox msgBox;
            msgBox.setText("Ты инвалид");
            msgBox.setWindowTitle("Ошибка авторизации");
            msgBox.exec();
        }
        return;
    }

    if (command == "logout") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            back();
        } else {
            // FUCK OH NO
        }
        return;
    }

    if (command == "disconnect") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            // exit(0);
        } else {
            // FUCK OH NO
        }
        return;
    }

    if (command == "create-room") {
        CreateRoomAnswer(answer);
        return;
    }

    if (command == "join-room") {
        JoinRoomAnswer(answer);
        return;
    }

    if (command == "money-info") {
        MoneyInfoAnswer(answer);
        return;
    }

    if (command == "add-money") {
        return;
    }
}

void Resolver::RoomBasicAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "leave") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            players_.clear();
            back();
        } else {
            // FUCK OH NO
        }
        return;
    }
}

void Resolver::RoomAdminAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "start") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            emit ShowActions();
            emit BlockActions();
            // DELETE START & LEAVE BUUTTOM
        } else {
            // FUCK OH NO
        }
        return;
    }
}

void Resolver::RoomGameAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "leave") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            back();
        } else {
            // FUCK OH NO
        }
        return;
    }
}

void Resolver::CreateRoomAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "on") {
        return;
    }

    if (status == "done") {
        globalInfo::RoomId = parametrs.get<uint64_t>("id");
        emit navigateTo(GAME_TAG);
        return;
    }

    if (status == "fail") {
        // FUCK!
    }
}

void Resolver::JoinRoomAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "on") {
        return;
    }

    if (status == "done") {
        our_server_position_ = parametrs.get<unsigned short>("position");
        navigateTo(GAME_TAG);
        return;
    }

    if (status == "fail") {
        // FUCK!
    }
}

void Resolver::MoneyInfoAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    globalInfo::Balance = parametrs.get<uint64_t>("money");
}

uint8_t Resolver::GetTablePos(const uint8_t &pos) {
    return (MAX_PLAYERS + pos - our_server_position_) % MAX_PLAYERS;
}

static std::vector<bool> GetAvailable(const std::string &str) {
    if (str == "raise-check") {
        return { false, false, true, true, false };
    }

    return { true, true, true, false, false };
}

void Resolver::GameAnswer(pt::ptree const &answer) {
//    if (answer == last_answer) {
//        return;
//    }

    if (answer.get<std::string>("command") != "game-status") {
        return;
    }

    auto parametrs = answer.get_child("parametrs");
    auto admin_pos = parametrs.get<uint8_t>("admin-pos");
    if (admin_pos == our_server_position_) {
        emit ShowStart();
    }

    if (!is_admin && admin_pos == our_server_position_) {
        is_admin = true;
        emit ShowStart();
    }

    auto gamestatus = parametrs.get_child("game-status");
    auto players = gamestatus.get_child("players");

    is_started = gamestatus.get<bool>("is-started");

    std::vector<resolver::Player> new_players;
    GetPlayers(players, new_players);

    if (is_started == false) {
        CheckPlayers(new_players);
        return;
    }

    players_.clear();
    players_ = std::move(new_players);

    if (first_msg) {
        first_msg = false;
        HandlePlayerCards();
        return;
    }

    auto current_cards_on_board = gamestatus.get<short>("num-cards-on-table");
    if (cards_on_board != current_cards_on_board) {
        cards_on_board = current_cards_on_board;
        qDebug("I am here");
        auto board_cards = gamestatus.get_child("board-сards");
        qDebug("I was here");
        HandleBoardCards(board_cards);
    }

    auto current_turn = GetTablePos(gamestatus.get<uint8_t>("current-turn"));
    auto min_bet = gamestatus.get<int>("big-blind-bet");
    emit SetMinBet(min_bet);
    emit SetMaxBet(10 * min_bet);

    emit ShowActions();
    if (current_turn == 0) {
        auto avaiable = gamestatus.get<std::string>("current-actions");
        emit AvaliableActions(GetAvailable(avaiable));
        emit UnBlockActions();
    }

    emit CurrentTurn(current_turn);

    auto winner_pos = gamestatus.get<uint8_t>("winner-position");
    if (winner_pos != WINNER_NOT_DEFINDED) {
        emit DisplayWinner(winner_pos);
        emit FlipAllCards();
    }
}

void Resolver::HandlePlayerCards() {
    for (auto &it : players_) {
        auto card_one = it.cards_in_hand[0];
        auto card_two = it.cards_in_hand[1];
        qDebug() << it.position;
        emit GiveCards(it.position, card_one.value, card_one.suit, 
                                    card_two.value, card_two.suit);
    }
}

void Resolver::GetPlayers(pt::ptree const &players, std::vector<resolver::Player> &new_players) {
    BOOST_FOREACH(const pt::ptree::value_type &vp, players) {
        const pt::ptree player = vp.second;
        resolver::Player current_player;
        current_player.name = player.get<std::string>("name");
        current_player.position = GetTablePos(player.get<uint8_t>("position"));
        current_player.money = 10;

        if (!is_started) {
            new_players.push_back(current_player);
            continue;
        }

        current_player.in_pot = player.get<bool>("in-pot");
        current_player.money = player.get<uint64_t>("current-stage-money-in-pot");  // NOT MONEY
        auto cards = player.get_child("cards");
        BOOST_FOREACH(const pt::ptree::value_type &vc, cards) {
            const pt::ptree card = vc.second;
            resolver::Card current_card;
            current_card.suit = card.get<uint8_t>("suit");
            current_card.value = card.get<uint8_t>("value");
            current_card.is_opened = card.get<bool>("is-opend");
            current_player.cards_in_hand.push_back(current_card);
        }
        players_.push_back(current_player);
        new_players.push_back(current_player);
    }
}

void Resolver::CheckPlayers(const std::vector<resolver::Player> &new_players) {
    for (size_t i = 0; i < players_.size(); ++i) {
        auto cit = players_.cbegin() + i;
        auto res = std::find_if(new_players.begin(), new_players.end(),
                      [cit](const resolver::Player &current) { return current.name == cit->name; });
        if (res == new_players.end()) {
            emit DeletePlayer(cit->position);
            players_.erase(cit);
            continue;
        }
    }

    for (auto &it : new_players) {
        auto res = std::find_if(players_.begin(), players_.end(),
                      [it](const resolver::Player &current) { return current.name == it.name; });
        if (res == players_.end()) {
            players_.push_back(it);
            emit DrawPlayer(it.position, it.name, it.money);
        }
    }
}


void Resolver::HandleBoardCards(pt::ptree const &board_cards) {
    emit DeleteAllCardsFromTable();
    BOOST_FOREACH(const pt::ptree::value_type &vb, board_cards) {
        const pt::ptree card = vb.second;
        auto suit = card.get<uint8_t>("suit");
        auto value = card.get<uint8_t>("value");
//        auto is_opened = card.get<bool>("is-opend");
        emit AddCardToTable(value, suit, true);
    }
}

void Resolver::Run() {
    while (Client->IsConnected()) {
        auto m = Client->GetLastMsg();
        std::cout << m;
        std::stringstream msg(m);
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        ParseAnswer(json_data);
    }
}
