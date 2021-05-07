#include "msgmaker.h"

#include <sstream>

#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

inline std::string MsgFabric::MsgFromPtree(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string MsgFabric::Autorisation(std::string const &login, std::string const &password) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("login", login);
    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "autorisation");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgFabric::Logout() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "logout");

    return MsgFromPtree(request);
}

std::string MsgFabric::Ping() {
    boost::property_tree::ptree request;

    request.put("command-type", "ping");
    request.put("command", "ping");

    return MsgFromPtree(request);
}

std::string MsgFabric::Disconnect() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "disconnect");

    return MsgFromPtree(request);
}

std::string MsgFabric::CreateRoom(std::string const &password) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "create-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgFabric::JoinRoom(uint64_t const &id, std::string const &password) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("id", id);
    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgFabric::StartGame() {
    boost::property_tree::ptree request;

    request.put("command-type", "room-admin");
    request.put("command", "start-game");

    return MsgFromPtree(request);
}

std::string MsgFabric::LeaveRoom() {
    boost::property_tree::ptree request;

    request.put("command-type", "room-basic");
    request.put("command", "leave");

    return MsgFromPtree(request);
}