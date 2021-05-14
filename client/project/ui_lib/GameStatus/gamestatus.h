#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <boost/property_tree/ptree.hpp>
#include "gamefragment.h"

class GameStatus {
public:
    GameStatus(GameFragment* game);
    ~GameStatus();

    void GetInfo(boost::property_tree::ptree status);


    void Run();
    void Stop();
private:
    GameFragment* Game;
};

#endif // GAMESTATUS_H
