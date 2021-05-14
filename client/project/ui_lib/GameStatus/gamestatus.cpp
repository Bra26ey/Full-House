#include "gamestatus.h"

GameStatus::GameStatus(GameFragment* game) : Game(game) {

}


void GameStatus::GetInfo(boost::property_tree::ptree status) {
    // status magic

////    if (пришла карта на стол )
//        Game->AddCardToTable(card.value, card.suite, up or down)


//      if (юзер коннектед или уже существует)
//        Game->DrawPlayer(id от 1 до 5, nickname, money)

//        if (кто то совершил какое то действие)
//            Game->
}


void GameStatus::Run() {

}
void GameStatus::Stop() {

}
