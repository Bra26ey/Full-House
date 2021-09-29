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
    Resolver();
    ~Resolver() = default;

    void Run();

signals:
    void WrongDataAutorisation();
    void WrongDataRegistration();
    void RightDataRegistration();
    void WrongDataRoomJoin();

    void DeleteAllPlayersCards();
    void DeletePlayer(size_t player_id);
    void DrawPlayer(size_t player_id, std::string nickname, size_t total_money);

    void SetMinBet(int minbet);
    void SetMaxBet(int maxbet);

    void EndGame(bool is_admin);

    void SetMoneyInBank(size_t money);
    void FlipTableCards();
    void DeleteAllCardsFromTable();
    void AddCardToTable(size_t value,  size_t suit, bool upsided);
    void FlipAllCards();

    void ShowActions();
    void BlockActions();
    void UnBlockActions();
    void ShowStart();

    void MakeDealer(size_t player_id);
    void DisplayWinner(size_t player_id);
    void DeleteWinnerDisplay();
    void CurrentTurn(size_t player_id);
    void GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2);
    void FlipCards(size_t player_id);
    void SetBet(size_t player_id, size_t bet);
    void SetFold(size_t player_id);
    void SetCall(size_t player_id);
    void SetRaise(size_t player_id, size_t bet);
    void SetCheck(size_t player_id);
    void SetMoney(size_t player_id, size_t money);
    void ClearStatus(size_t player_id);
    void AvaliableActions(std::vector<bool> buttons);
    void ClearBank();



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

    void CheckPlayers(const std::vector<resolver::Player> &new_players);
    void HandleBoardCards(pt::ptree const &board_cards);
    void HandlePlayerCards();

    void HandleInitGame(const pt::ptree &gamestatus);
    void HandleAdminChange();
    void HandlePlayerChange(const pt::ptree &gamestatus);
    void HandleEndOfGame(const uint8_t &winner_pos);
    void HandleTurnChange(const uint8_t &current_turn, const pt::ptree &gamestatus);
    void HandleActions(const pt::ptree &gamestatus);

    uint8_t GetTablePos(const uint8_t &pos);
    void GetPlayers(pt::ptree const &players, std::vector<resolver::Player> &players_vec);
    void CheckPlayers(pt::ptree const &players);
    void ClearAllStatus();


private:
    uint8_t our_server_position_;
    uint8_t admin_position_;
    uint8_t cards_on_board_;
    uint8_t current_turn_;
    uint16_t num_actions_;
    bool first_msg_;
    bool is_admin_;
    bool is_started_;
    bool winner_displayed;
    uint8_t winner_pos;
    std::vector<resolver::Player> players_;
};
