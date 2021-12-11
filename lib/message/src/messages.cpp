#include "messages.hpp"

#include <sstream>
#include <string>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

std::string Message::message_phtee(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string MessageClient::msg() {
    pt::ptree request;

    request.put("command_type", "ping");
    request.put("command", "ping");

    return message_phtee(request);
}

std::string MessageClient::disconnect() {
    pt::ptree request;

    request.put("command_type", "basic");
    request.put("command", "disconnect");

    return message_phtee(request);
}

std::string MessageClient::create_room() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "on");

    request.put("command_type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageClient::join_room(const size_t &room_id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "on");
    parametrs.put("id", room_id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::create_room_done(const int &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", id);
    parametrs.put("status", "on");
    request.put("command-type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::create_room_failed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");
    request.put("command-type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::accept_room_done(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", id);
    request.put("command", "accepted");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::start_game(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command_type", "room_admin");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::start_game_failed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command-type", "room-room_admin-answer");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageClient::error() {
    pt::ptree request;

    request.put("command_type", "error");
    request.put("command", "error");

    return message_phtee(request);
}

std::string MessageServer::connected(const std::string& ids, const std::string& ips, const int &role) {
    pt::ptree parametrs;
    pt::ptree game;
    pt::ptree request;

    pt::ptree id;
    pt::ptree ip;

    id.put("ids", ids);
    ip.put("ips", ips);

    parametrs.add_child("ids", id);
    parametrs.add_child("ips", ip);
    request.add_child("parametrs", parametrs);

    game.put("role", role);
    game.put("status", "ON");

    request.put("command_type", "basic");
    request.put("command", "game");

    request.add_child("game", game);

    return message_phtee(request);
}

std::string MessageServer::msg(const std::string& ids, const std::string& names) {
    pt::ptree parametrs;
    pt::ptree id;
    pt::ptree name;
    pt::ptree request;

    id.put("ids", ids);
    name.put("names", names);

    request.put("command_type", "basic");
    request.put("command", "pre_game");

    parametrs.add_child("ids", id);
    parametrs.add_child("name", name);
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}


//std::string MessageServer::game_start_______________--(const int role) {
//    pt::ptree request;
//    pt::ptree parametrs;
//
//    request.put("command", "game_start");
//    parametrs.put("role", role);
//
//    request.add_child("parametrs", parametrs);
//    return std::string();
//}

std::string MessageServer::join_room_failed(const int &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");
    parametrs.put("id", id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageServer::leave_room_done() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command_type", "basic_room");
    request.put("command", "leave");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}