#include "msgmaker.h"

#include <sstream>

#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

MsgFabric::~MsgFabric() {}

inline std::string MsgFabric::MsgFromPtree(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}



std::string MsgClient::Autorisation(std::string const &login, std::string const &password) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("login", login);
    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "autorisation");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::Registration(std::string const &login, std::string const &password) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("login", login);
    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "registration");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::Logout() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "logout");

    return MsgFromPtree(request);
}

std::string MsgClient::Ping() {
    boost::property_tree::ptree request;

    request.put("command-type", "ping");
    request.put("command", "ping");

    return MsgFromPtree(request);
}

std::string MsgClient::Disconnect() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "disconnect");

    return MsgFromPtree(request);
}

std::string MsgClient::CreateRoom(std::string const &password) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "create-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::CreateRoomResault() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "create-room-result");

    return MsgFromPtree(request);
}

std::string MsgClient::JoinRoom(uint64_t const &id, std::string const &password) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("id", id);
    parametrs.put("password", password);

    request.put("command-type", "basic");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::JoinRoomResult() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "join-room-result");

    return MsgFromPtree(request);
}

std::string MsgClient::StartGame() {
    boost::property_tree::ptree request;

    request.put("command-type", "room-admin");
    request.put("command", "start-game");

    return MsgFromPtree(request);
}

std::string MsgClient::GetGameStatus() {
    boost::property_tree::ptree request;

    request.put("command-type", "game");
    request.put("command", "status-request");

    return MsgFromPtree(request);
}

std::string MsgClient::GameRaise(uint64_t const &sum) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("action-type", "raise");
    parametrs.put("sum", sum);

    request.put("command-type", "game");
    request.put("command", "action");

    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::GameCall() {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("action-type", "call");

    request.put("command-type", "game");
    request.put("command", "action");

    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::GameCheck() {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("action-type", "check");

    request.put("command-type", "game");
    request.put("command", "action");

    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::GameFold() {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("action-type", "fold");

    request.put("command-type", "game");
    request.put("command", "action");

    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::LeaveRoom() {
    boost::property_tree::ptree request;

    request.put("command-type", "room-basic");
    request.put("command", "leave");

    return MsgFromPtree(request);
}

std::string MsgClient::AddMoney(uint64_t const &money) {
    boost::property_tree::ptree parametrs;
    boost::property_tree::ptree request;

    parametrs.put("money", money);

    request.put("command-type", "basic");
    request.put("command", "add-money");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgClient::MoneyInfo() {
    boost::property_tree::ptree request;

    request.put("command-type", "basic");
    request.put("command", "money-info");

    return MsgFromPtree(request);
}



std::string MsgServer::AutorisationDone() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "basic-answer");
    request.put("command", "autorisation");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::AutorisationFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "basic-answer");
    request.put("command", "autorisation");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::RegistrationDone() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "basic-answer");
    request.put("command", "registration");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::RegistrationFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "basic-answer");
    request.put("command", "registration");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::Logout() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "basic-answer");
    request.put("command", "logout");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::Ping() {
    boost::property_tree::ptree request;

    request.put("command-type", "ping");
    request.put("command", "ping");

    return MsgFromPtree(request);
}

std::string MsgServer::Error() {
    pt::ptree request;

    request.put("command-type", "error");
    request.put("command", "error");

    return MsgFromPtree(request);
}

std::string MsgServer::Disconnect() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "basic-answer");
    request.put("command", "disconnect");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::CreateRoomOn() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "on");

    request.put("command-type", "basic-answer");
    request.put("command", "create-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::CreateRoomDone(uint64_t const &id, std::string const &password) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");
    parametrs.put("id", id);
    parametrs.put("password", password);


    request.put("command-type", "basic-answer");
    request.put("command", "create-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::CreateRoomFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "basic-answer");
    request.put("command", "create-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::JoinRoomOn(uint64_t const &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "on");
    parametrs.put("id", id);

    request.put("command-type", "basic-answer");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}


std::string MsgServer::JoinRoomDone(uint64_t const &id, uint8_t const &position) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");
    parametrs.put("id", id);
    parametrs.put("position", position);

    request.put("command-type", "basic-answer");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::JoinRoomFailed(uint64_t const &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");
    parametrs.put("id", id);

    request.put("command-type", "basic-answer");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::LeaveRoomDone() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "room-basic-answer");
    request.put("command", "leave");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::LeaveRoomFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "room-basic-answer");
    request.put("command", "leave");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::StartGameDone() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "room-admin-answer");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::StartGameFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "room-admin-answer");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::GameStatus(pt::ptree const &game_status, uint8_t const &admin_pos) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("admin-pos", admin_pos);
    parametrs.add_child("game-status", game_status);

    request.put("command-type", "game-answer");
    request.put("command", "game-status");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::AddMoneyDone() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command-type", "basic-answer");
    request.put("command", "add-money");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::AddMoneyFailed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "basic-answer");
    request.put("command", "add-money");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}

std::string MsgServer::MoneyInfo(uint64_t const &sum) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("money", sum);

    request.put("command-type", "basic-answer");
    request.put("command", "money-info");
    request.add_child("parametrs", parametrs);

    return MsgFromPtree(request);
}
