#include <boost/foreach.hpp>

#include <QMessageBox>

#include "screenfactory.h"

ParseAnswer(pt::ptree const &answer) {
    auto command_type = answer.get<std::string>("command-type");

    if (command_type == "ping") {
        return;
    }

    if (command_type == "error") {
        return;
    }

    if (command_type == "basic-answer") {
        BasicAnswer(answer);
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

BaseAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "autorisation") {
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
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
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
            // YEHOOOO LOGOUT
        } else {
            // FUCK OH NO
        }
        return;
    }
    
    if (command == "disconnect") {
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
            // YEHOOOO WE FREE
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
    
}

RoomBasicAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");
    
    if (command == "leave") {
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
            // YEHOOOO WE FREE
        } else {
            // FUCK OH NO
        }
        return;
    }
}

RoomAdminAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");
    
    if (command == "start") {
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
            // YEHOOOO WE FREE
        } else {
            // FUCK OH NO
        }
        return;
    }
}

RoomGameAnswer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");
    
    if (command == "start") {
        if (command.get_child("parametrs").get<std::string>(status) == "done") {
            // YEHOOOO WE FREE
        } else {
            // FUCK OH NO
        }
        return;
    }
}

CreateRoomAnswer(pt::ptree const &answer) {
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

JoinRoomAnswer(pt::ptree const &answer) {
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

GameAnswer(pt::ptree const &answer) {
    if (answer == last_answer) {
        return;
    }

    if (answer.get<std::string>("command" != game-status) {
        return;
    }

    auto parametrs = answer.get_child("parametrs");
    auto players = parametrs.get_child("players");
    
    std::vector<Player> players_vector;
    
    BOOST_FOREACH(const pt::ptree& player, players)) {
        Player current_player;
        current.in_pot = player.get<bool>("in-pot");
        current.money = player.get<uint64_t>("current-stage-money-in-pot");
        current.position = player.get<uint64_t>("position");
        auto cards = player.get_child("cards");
        BOOST_FOREACH(const pt::ptree& card, cards)) {
            Card current_card;
            current_card.suit = card.get<uint64_t>("suit");
            current_card.value = card.get<uint64_t>("value");
            current_card.is_opend = card.get<bool>("is-opend");
            current_player.cards.push_back(card)
        }
        players_vector.push_back(current_player);
    }


}


