#pragma once

#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

class MsgFabric {
 public:
    virtual ~MsgFabric() = 0;
 protected:
    static std::string MsgFromPtree(boost::property_tree::ptree const &request);
};

class MsgClient : public MsgFabric {
 public:
    static std::string Autorisation(std::string const &login, std::string const &password);
    static std::string Registration(std::string const &login, std::string const &password);

    static std::string Logout();
    static std::string Ping();
    static std::string Disconnect();

    static std::string CreateRoom(std::string const &password);
    static std::string CreateRoomResault();

    static std::string JoinRoom(uint64_t const &id, std::string const &password);
    static std::string JoinRoomResult();

    static std::string LeaveRoom();

    static std::string StartGame();

    static std::string GetGameStatus();

    static std::string GameRaise(uint64_t const &sum);
    static std::string GameCall();
    static std::string GameFold();
    static std::string GameCheck();
};

class MsgServer : public MsgFabric {
 public:
    static std::string AutorisationDone();
    static std::string AutorisationFaild();

    static std::string RegistrationDone();
    static std::string RegistrationFailed();

    static std::string Logout();
    static std::string Ping();
    static std::string Error();
    static std::string Disconnect();

    static std::string CreateRoomOn();
    static std::string CreateRoomDone(uint64_t const &id, std::string const &password);
    static std::string CreateRoomFailed();

    static std::string JoinRoomOn(uint64_t const &id);
    static std::string JoinRoomDone(uint64_t const &id, uint8_t const &position);
    static std::string JoinRoomFaild(uint64_t const &id);

    static std::string LeaveRoomDone();
    static std::string LeaveRoomFailed();

    static std::string StartGameDone();
    static std::string StartGameFailed();

    static std::string GameStatus(boost::property_tree::ptree const &game_status, uint8_t const &admin_pos);
};
