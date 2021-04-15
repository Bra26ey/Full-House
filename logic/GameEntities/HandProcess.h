//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HANDPROCESS_H
#define FULL_HOUSE_HANDPROCESS_H

#include <list>
#include <memory>

#include "Player.h"
#include "Board.h"
#include "HandConfiguration.h"
#include "Logger.h"


constexpr int DECK_SIZE = 52;

class HandProcess {
public:
    // HandProcess();
    void DealCards();  // TODO: using ShuffleDeck
    void Preflop();  // TODO: using GameStage
    void Flop();  // TODO: using GameStage
    void Turn();  // TODO: using GameStage
    void River();  // TODO: using GameStage
    void PotDistribution();  // TODO: using HandValue from Player and sort for all players to determine winner

private:
    std::vector<Card> deck;
    Board board_;
    std::list<std::unique_ptr<Player> > players_;

    std::shared_ptr<Logger> logger_;

    std::list<std::unique_ptr<Player> >::iterator iter_;
    unsigned int pos_of_raiser_;


    void ShuffleDeck();
    void GameStage();  // TODO: while loop pos_of_raiser == start_pos
};



#endif //FULL_HOUSE_HANDPROCESS_H
