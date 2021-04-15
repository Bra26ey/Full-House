//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HUMANPLAYER_H
#define FULL_HOUSE_HUMANPLAYER_H

#include "Player.h"
#include "Profile.h"

struct HumanPlayer: public Player, public Profile {
    void IsConnected();  // TODO: need to stop the game to wait if player disconnected. Using callback maybe?
};

#endif //FULL_HOUSE_HUMANPLAYER_H
