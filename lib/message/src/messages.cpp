#include "messages.hpp"

#include <sstream>

#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

std::string Message::message_phtee(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string MessageClient::msg() {
    pt::ptree request;

    request.put("command_type", "message");
    request.put("command", "message");

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

std::string MessageServer::create_room_done(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", id);
    request.put("command", "room_is_created");
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

    parametrs.put("id", id);
    request.put("command", "start_game");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string MessageClient::error() {
    pt::ptree request;

    request.put("command_type", "error");
    request.put("command", "error");

    return message_phtee(request);
}

std::string MessageServer::connected(const std::vector<std::vector<std::string>> &users_ip) {
    pt::ptree parametrs;
    pt::ptree request;

    for (const auto &i: users_ip) {
        parametrs.put("name", i[0]);
        parametrs.put("ip", i[1]);

    }

    request.put("command", "ips");
    request.add_child("parametrs", parametrs);

    return std::string();
}


std::string MessageServer::game_start(const int role) {
    pt::ptree request;
    pt::ptree parametrs;

    request.put("command", "game_start");
    parametrs.put("role", role);

    request.add_child("parametrs", parametrs);
    return std::string();
}

std::string MessageServer::join_room_failed(const int &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");
    parametrs.put("id", id);

    request.put("command_type", "basic-answer");
    request.put("command", "join-room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}