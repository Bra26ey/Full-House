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
    void DeletePlayer(size_t player_id);
    void DrawPlayer(size_t player_id, std::string nickname, size_t total_money);

    void SetMinBet(int minbet);
    void SetMaxBet(int maxbet);

    void EndGame(bool is_admin);
    void JoinNotAdmin();

    void FlipTableCards();
    void DeleteAllCardsFromTable();
    void AddCardToTable(size_t value,  size_t suit, bool upsided);
    void FlipAllCards();

    void ShowActions();
    void BlockActions();
    void UnBlockActions();
    void HideStart();

    void MakeDealer(size_t player_id);
    void DisplayWinner(size_t player_id);
    void CurrentTurn(size_t player_id);
    void GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2);
    void FlipCards(size_t player_id);
    void SetBet(size_t player_id, size_t bet);
    void SetFold(size_t player_id);
    void SetCall(size_t player_id);
    void SetRaise(size_t player_id);
    void SetCheck(size_t player_id);
    void ClearStatus(size_t player_id);
    void AvaliableActions(std::vector<bool> buttons);

    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();
    void onCallPressed();

    void onLeavePressed();
    void onSettingsPressed();
    void onStartPressed();

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
