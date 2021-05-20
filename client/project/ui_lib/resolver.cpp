#include "client_impl.h"
#include "resolver.h"


using namespace screens;

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

    if (command == "autorisation") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
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
            exit(0);
        } else {
            // FUCK OH NO
        }
        return;
    }

    if (command == "create-room") {
        CreateRoomAnswer(answer);
        navigateTo(GAME_TAG);
        return;
    }
    
    if (command == "join-room") {
        JoinRoomAnswer(answer);
        navigateTo(GAME_TAG);
        return;
    }
    
}

void Resolver::RoomBasicAnswer(pt::ptree const &answer) {
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

void Resolver::RoomAdminAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");
    
    if (command == "start") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            // YEHOOOO WE FREE
        } else {
            // FUCK OH NO
        }
        return;
    }
}

void Resolver::RoomGameAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");
    
    if (command == "start") {
        if (answer.get_child("parametrs").get<std::string>("status") == "done") {
            // YEHOOOO WE FREE
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
        // OMG IT'S DONE!
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
        // OMG IT'S DONE!
    }

    if (status == "fail") {
        // FUCK!
    }
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

    // Min & Max Bet SetMinBet(value) SetMaxBet(value)
    // Dealer or not, MakeDealer(id)
    // Current Turn Player CurrentTurn(id)
    // Money in bank(pot) складывается из бетов вроде как автоматически
    // Cards on the table AddCardToTable(size_t value, size_t suit, bool upsided) / FlipTableCards / DeleteTableCards
    // Available actions
    // Won Label DisplayWinner(id) / DeleteWinnerLabel(void)
    // GameStarted? ShowActions нужно написать сочетания кнопок, потому что сейчас такой функции нет
    
    std::vector<resolver::Player> players_vector;
    
//    BOOST_FOREACH(const pt::ptree& player, players) {
//        // Player name ???

//        Player current_player;
//        current.in_pot = player.get<bool>("in-pot");
//        current.money = player.get<uint64_t>("current-stage-money-in-pot");
//        current.position = player.get<uint8_t>("position");
//        auto cards = player.get_child("cards");
//        BOOST_FOREACH(const pt::ptree& card, cards) {
//            Card current_card;
//            current_card.suit = card.get<uint8_t>("suit");
//            current_card.value = card.get<uint8_t>("value");
//            current_card.is_opend = card.get<bool>("is-opend");
//            current_player.cards.push_back(card)
//        }
//        players_vector.push_back(current_player);
//    }
}

void Resolver::Run() {
    while(Client->IsConnected()) {
        std::stringstream msg(Client->GetLastMsg());
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        ParseAnswer(json_data);
    }
}
