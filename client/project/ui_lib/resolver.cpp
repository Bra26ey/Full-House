#include "resolver.h"
#include "client_impl.h"


using namespace screens;

constexpr uint8_t WINNER_NOT_DEFINDED = 10;
constexpr uint8_t MAX_PLAYERS = 6;

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
            gamefragment_ = nullptr;
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
            gamefragment_->ShowActions();
            gamefragment_->BlockActions();
            gamefragment_->HideStart();
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
        // OMG IT'S TRYING!
        return;
    }

    if (status == "done") {
        navigateTo(GAME_TAG);
        gamefragment_ = dynamic_cast<GameFragment*>(Front());
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
        // OMG IT'S TRYING!
        return;
    }

    if (status == "done") {
        our_server_position_ = parametrs.get<uint8_t>("position");
        navigateTo(GAME_TAG);
        gamefragment_ = dynamic_cast<GameFragment*>(Front());
        gamefragment_->JoinNotAdmin();
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
    auto players = parametrs.get_child("players");

    auto current_turn = GetTablePos(parametrs.get<uint8_t>("current-turn"));

    if (parametrs.get<bool>("is-started") == false) {
        gamefragment_->BlockActions();
        return;
    }

    auto min_bet = parametrs.get<int>("big-blind-bet");
    gamefragment_->SetMinBet(min_bet);
    gamefragment_->SetMaxBet(10 * min_bet);

    gamefragment_->ShowActions();
    if (current_turn == 0) {
        auto avaiable = parametrs.get<std::string>("current-actions");
        gamefragment_->AvaliableActions(GetAvailable(avaiable));
        gamefragment_->UnBlockActions();
    }

    gamefragment_->CurrentTurn(current_turn);

    auto winner_pos = parametrs.get<uint8_t>("winner-pos");
    if (winner_pos != WINNER_NOT_DEFINDED) {
        gamefragment_->DisplayWinner(winner_pos);
    }

    // boost::property_tree::ptree board_cards;
    // for (auto &it : board_.cards) {
    //     auto card = GetCardStatus(it);
    //     board_cards.add_child("", card);
    // }
    // status.add_child("board-сards", board_cards);

    auto board_cards = parametrs.get_child("board-сards");




    // Min & Max Bet SetMinBet(value) SetMaxBet(value)
    // Dealer or not, MakeDealer(id)
    // Current Turn Player CurrentTurn(id)
    // Money in bank(pot) складывается из бетов вроде как автоматически
    // Cards on the table AddCardToTable(size_t value, size_t suit, bool upsided) / FlipTableCards / DeleteTableCards
    // Available actions
    // Won Label DisplayWinner(id) / DeleteWinnerLabel(void)
    // GameStarted? ShowActions нужно написать сочетания кнопок, потому что сейчас такой функции нет

    std::vector<resolver::Player> players_vector;

    BOOST_FOREACH(const pt::ptree::value_type &vp, players) {
       const pt::ptree player = vp.second;

       resolver::Player current_player;
       current_player.name = player.get<std::string>("name");
       current_player.in_pot = player.get<bool>("in-pot");
       current_player.money = player.get<uint64_t>("current-stage-money-in-pot");
       current_player.position = player.get<uint8_t>("position");
       gamefragment_->DrawPlayer(GetTablePos(current_player.position), current_player.name, current_player.money);
       auto cards = player.get_child("cards");
       BOOST_FOREACH(const pt::ptree::value_type &vc, cards) {
           const pt::ptree card = vc.second;
           resolver::Card current_card;
           current_card.suit = card.get<uint8_t>("suit");
           current_card.value = card.get<uint8_t>("value");
           current_card.is_opened = card.get<bool>("is-opend");
           current_player.cards_in_hand.push_back(current_card);
       }
       players_vector.push_back(current_player);
    }
}

void Resolver::Run() {
    while (Client->IsConnected()) {
        std::stringstream msg(Client->GetLastMsg());
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        ParseAnswer(json_data);
    }
}
