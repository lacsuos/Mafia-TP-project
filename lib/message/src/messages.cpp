#include "messages.hpp"

#include <sstream>

#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

std::string Message::message_phtee(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string Message::msg() {
    pt::ptree request;

    request.put("command", "message_from_server");

    return message_phtee(request);
}

std::string Message::disconnect() {
    pt::ptree request;

    request.put("command", "disconnected from server");

    return message_phtee(request);
}

std::string Message::create_room() {
    pt::ptree request;

    request.put("command", "created room");

    return message_phtee(request);
}

std::string Message::join_room(const size_t &room_id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", room_id);
    request.put("command", "joined_room");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string Message::create_room_done(const size_t &id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", id);
    request.put("command", "room_is_created");
    request.add_child("parametrs", parametrs);

    return message_phtee(request);
}

std::string Message::connected(const std::vector<std::vector<std::string>> &users_ip) {
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
