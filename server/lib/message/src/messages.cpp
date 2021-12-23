#include "messages.hpp"

#include <sstream>
#include <string>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

std::string Message::message_ptree(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string MessageClient::msg() {
    pt::ptree request;

    request.put("command_type", "ping");
    request.put("command", "ping");

    return message_ptree(request);
}

std::string MessageClient::disconnect() {
    pt::ptree request;

    request.put("command_type", "basic");
    request.put("command", "disconnect");

    return message_ptree(request);
}

std::string MessageClient::create_room() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "in_process");

    request.put("command_type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageClient::join_room(const size_t &room_id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "in_process");
    parametrs.put("id", room_id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::create_room_done(const int &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", id);
    parametrs.put("status", "done");

    request.put("command_type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::create_room_failed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command_type", "basic");
    request.put("command", "create_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::join_room_done(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");
    parametrs.put("id", id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::start_game(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command_type", "room_admin");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::start_game_failed() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");

    request.put("command_type", "room_admin");
    request.put("command", "start");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageClient::error() {
    pt::ptree request;

    request.put("command_type", "error");
    request.put("command", "error");

    return message_ptree(request);
}

std::string
MessageServer::connected(const std::string &ids, const std::string &ips, const int &role,
                         const bool &is_alive, const bool &is_sleep, const std::string &status) {
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
    if (is_alive) {
        game.put("status_is_alive", "ON");
    } else {
        game.put("status_is_alive", "OFF");
    }
    if (is_sleep) {
        game.put("status_is_sleep", "ON");
    } else {
        game.put("status_is_sleep", "OFF");
    }

    game.put("status", status);

    request.put("command_type", "basic");
    request.put("command", "game");

    request.add_child("game", game);

    return message_ptree(request);
}

std::string MessageServer::msg(const std::string &ids, const std::string &names) {
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

    return message_ptree(request);
}


std::string MessageServer::join_room_failed(const int &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "fail");
    parametrs.put("id", id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::leave_room_done() {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "done");

    request.put("command_type", "basic_room");
    request.put("command", "leave");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}

std::string MessageServer::finish_game() {
    pt::ptree request;

    request.put("command_type", "basic_room");
    request.put("command", "finish_game");

    return message_ptree(request);
}

std::string MessageServer::day_accepted() {
    pt::ptree request;

    request.put("command_type", "basic_room");
    request.put("command", "day_accepted");

    return message_ptree(request);
}

std::string MessageServer::nigth_accepted(const int &killed_id) {
    pt::ptree request;
    pt::ptree parametrs;


    parametrs.put("vote_id", killed_id);

    request.put("command_type", "basic_room");
    request.put("command", "day_accepted");
    request.add_child("parametrs", parametrs);


    return message_ptree(request);
}

std::string MessageServer::vote_accepted(const int &id) {
    pt::ptree request;
    pt::ptree parametrs;

    parametrs.put("vote_id", id);

    request.put("command_type", "basic_room");
    request.put("command", "day_accepted");

    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}
