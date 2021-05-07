#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

class MsgFabric {
 public:
    std::string Autorisation(std::string const &login, std::string const &password);
    std::string Logout();
    std::string Ping();
    std::string Disconnect();
    std::string CreateRoom(std::string const &password);
    std::string JoinRoom(uint64_t const &id, std::string const &password);
    std::string StartGame();
    std::string LeaveRoom();

 private:
    std::string MsgFromPtree(boost::property_tree::ptree const &request);
};
