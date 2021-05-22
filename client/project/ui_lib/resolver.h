#pragma once
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <QMessageBox>

#include "screenfactory.h"
#include "gamefragment.h"



namespace pt = boost::property_tree;

namespace resolver {

struct Card {
    uint8_t suit;
    uint8_t value;
    bool is_opened;
};


struct Player {
    std::string name;
    bool in_pot;
    uint64_t money;
    uint8_t position;
    std::vector<Card> cards_in_hand;
};

}  // namespace resolver



class Resolver : public BaseFragment {
    Q_OBJECT
public:
    Resolver() : gamefragment_(nullptr),  our_server_position_(0), first_msg(true) {};
    ~Resolver() = default;

    void Run();

signals:
    void AddCardToTable(const int value, const int suit, const int upsided);
    void ShowActions();
    void DrawPlayer(int player_id, std::string nickname, int total_money);

private:
    void ParseAnswer(pt::ptree const &answer);
    void BaseAnswer(pt::ptree const &answer);
    void RoomBasicAnswer(pt::ptree const &answer);
    void RoomAdminAnswer(pt::ptree const &answer);
    void RoomGameAnswer(pt::ptree const &answer);
    void CreateRoomAnswer(pt::ptree const &answer);
    void JoinRoomAnswer(pt::ptree const &answer);
    void MoneyInfoAnswer(pt::ptree const &answer);
    void GameAnswer(pt::ptree const &answer);

    void HandleBoardCards(pt::ptree const &board_cards);

    uint8_t GetTablePos(const uint8_t &pos);
    void GetPlayers(pt::ptree const &players, std::vector<resolver::Player> &players_vec);

private:
    uint8_t our_server_position_;
    bool first_msg;
    std::vector<resolver::Player> players_;
};
