#include "resolver.h"

#include <algorithm>
#include <QDebug>
#include <QApplication>

#include "client_impl.h"

using namespace screens;

constexpr uint8_t NOT_DEFINDED = 10;
constexpr uint8_t MAX_PLAYERS = 6;
constexpr uint8_t MAX_CARDS = 2;

Resolver::Resolver() : our_server_position_(0), cards_on_board_(0),
                       current_turn_(MAX_PLAYERS), first_msg_(true), is_admin_(false),
                       is_started_ (false) {}

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
            emit RightDataRegistration();
            emit back();
        } else {
            emit WrongDataRegistration();
        }
        return;
    }

    if (command == "autorisation") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            emit navigateTo(MAIN_TAG);
        } else {
            emit WrongDataAutorisation();
        }
        return;
    }

    if (command == "logout") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            emit back();
        } else {
            // error
        }
        return;
    }

    if (command == "disconnect") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            // exit(0);
        } else {
            // error
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
            is_admin_ = false;
            is_started_ = false;
            first_msg_ = true;
            players_.clear();
            emit back();
        } else {
            // error
        }
        return;
    }
}

void Resolver::RoomAdminAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "start") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
        } else {
            // error
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
        our_server_position_ = 0;
        admin_position_ = 0;
        cards_on_board_ = 0;
        current_turn_ = 0;
        num_actions_ = 0;

        winner_displayed = false;
        is_admin_ = false;
        is_started_ = false;
        first_msg_ = true;
        players_.clear();
        globalInfo::RoomId = parametrs.get<uint64_t>("id");
        emit navigateTo(GAME_TAG);
        return;
    }

    if (status == "fail") {
        // error
    }
}

void Resolver::JoinRoomAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "on") {
        return;
    }

    if (status == "done") {
        our_server_position_ = 0;
        admin_position_ = 0;
        cards_on_board_ = 0;
        current_turn_ = 0;
        num_actions_ = 0;

        winner_displayed = false;
        is_admin_ = false;
        is_started_ = false;
        first_msg_ = true;
        players_.clear();
        our_server_position_ = parametrs.get<uint8_t>("position");
        emit navigateTo(GAME_TAG);
        return;
    }

    if (status == "fail") {
        emit WrongDataRoomJoin();
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
    if (answer.get<std::string>("command") != "game-status") {
        return;
    }

    auto parametrs = answer.get_child("parametrs");
    auto gamestatus = parametrs.get_child("game-status");
    is_started_ = gamestatus.get<bool>("is-started");

    auto admin_pos = parametrs.get<uint8_t>("admin-pos");
    if (!is_admin_ && admin_pos == our_server_position_) {
        HandleAdminChange();
    }

    HandlePlayerChange(gamestatus);

    auto winner_pos = gamestatus.get<uint8_t>("winner-position");
    if (winner_pos != NOT_DEFINDED) {
        HandleActions(gamestatus);
        HandleEndOfGame(winner_pos);
    }


    if (!is_started_) {
        return;
    }

    auto bank = gamestatus.get<size_t>("bank");
    emit SetMoneyInBank(bank);

    if (first_msg_) {
        HandleInitGame(gamestatus);
    }

    auto num_actions = gamestatus.get<uint16_t>("num-actions");
    if (num_actions_ != num_actions) {
        auto current_turn = GetTablePos(gamestatus.get<uint8_t>("current-turn"));
        HandleTurnChange(current_turn, gamestatus);
        num_actions_ = num_actions;
    }

    auto current_cards_on_board = gamestatus.get<short>("num-cards-on-table");
    if (cards_on_board_ != current_cards_on_board && current_cards_on_board != 0) {
        cards_on_board_ = current_cards_on_board;
        auto board_cards = gamestatus.get_child("board-сards");
        HandleBoardCards(board_cards);
    }
}

void Resolver::HandleAdminChange() {
    is_admin_ = true;
    emit ShowStart();
}

void Resolver::HandleInitGame(const pt::ptree &gamestatus) {
    emit ClearBank();
    num_actions_ = 0;
    current_turn_ = NOT_DEFINDED;
    first_msg_ = false;
    winner_displayed = false;
    emit DeleteWinnerDisplay();
    emit ClearAllStatus();
    emit DeleteAllCardsFromTable();
    emit DeleteAllPlayersCards();
    HandlePlayerCards();
    auto min_bet = gamestatus.get<int>("big-blind-bet");
    emit ShowActions();
    emit BlockActions();
    emit SetMinBet(min_bet);
    emit SetMaxBet(10 * min_bet);
}

void Resolver::HandleTurnChange(const uint8_t &current_turn, const pt::ptree &gamestatus) {
    current_turn_ = current_turn;
    emit CurrentTurn(current_turn_);
    if (current_turn_ == 0) {
        auto avaiable = gamestatus.get<std::string>("current-actions");
        auto bank = gamestatus.get<size_t>("bank");
        emit SetMoneyInBank(bank);
        emit AvaliableActions(GetAvailable(avaiable));
        emit UnBlockActions();
    }
    HandleActions(gamestatus);
}

void Resolver::HandleActions(const pt::ptree &gamestatus) {
    auto last_command = gamestatus.get_child("last-action");
    auto position = last_command.get<uint8_t>("position");
    position = GetTablePos(position);
    if (position == NOT_DEFINDED) {
        return;
    }

    auto action = last_command.get<std::string>("command");
    if (action == "fold") {
        emit SetFold(position);
        return;
    }

    if (action == "call") {
        emit SetCall(position);
        return;
    }

    if (action == "raise") {
        auto sum = last_command.get<int>("sum");
        emit SetRaise(position, sum);
        return;
    }

    if (action == "check") {
        emit SetCheck(position);
        return;
    }
}

void Resolver::HandleEndOfGame(const uint8_t &winner_pos) {
    if (winner_displayed == false) {
        auto converted_winner = GetTablePos(winner_pos);
        emit DisplayWinner(converted_winner);
        winner_displayed = true;
    }
    emit FlipAllCards();
    emit BlockActions();
    if (is_admin_) {
        emit ShowStart();
    }
    first_msg_ = true;
}

void Resolver::HandlePlayerChange(const pt::ptree &gamestatus) {
    auto players = gamestatus.get_child("players");
    std::vector<resolver::Player> new_players;
    GetPlayers(players, new_players);

    if (is_started_ == false) {
        CheckPlayers(new_players);
        return;
    }

    players_.clear();
    players_ = std::move(new_players);
}

void Resolver::HandlePlayerCards() {
    for (auto &it : players_) {
        auto card_one = it.cards_in_hand[0];
        auto card_two = it.cards_in_hand[1];
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
        current_player.money = player.get<int>("money");

        if (current_player.position == 0) {
            globalInfo::Balance = current_player.money;
            emit SetMaxBet(current_player.money);
        }

        if (!is_started_) {
            new_players.push_back(current_player);
            continue;
        }

        emit SetMoney(current_player.position, current_player.money);
        current_player.in_pot = player.get<bool>("in-pot");
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

void Resolver::ClearAllStatus() {
    for (auto &it : players_) {
        emit ClearStatus(it.position);
    }
}

void Resolver::HandleBoardCards(pt::ptree const &board_cards) {
    emit DeleteAllCardsFromTable();
    BOOST_FOREACH(const pt::ptree::value_type &vb, board_cards) {
        const pt::ptree card = vb.second;
        auto suit = card.get<uint8_t>("suit");
        auto value = card.get<uint8_t>("value");
        emit AddCardToTable(value, suit, true);
    }
}

void Resolver::Run() {
    while (Client->IsConnected()) {
        auto m = Client->GetLastMsg();
//        std::cout << m;
        std::stringstream msg(m);
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        ParseAnswer(json_data);
    }
}
