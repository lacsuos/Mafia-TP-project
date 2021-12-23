#include "messages.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;

std::string Message::message_ptree(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string MessageClient::ping() {
    boost::property_tree::ptree request;

    request.put("command-type", "ping");
    request.put("command", "ping");

    return message_ptree(request);
}

std::string MessageClient::disconnect() {
    boost::property_tree::ptree request;

    request.put("command-type", "disconnect");
    request.put("command", "disconnect");

    return message_ptree(request);
}

std::string MessageClient::create_room() {
    pt::ptree request;

    request.put("command_type", "basic");
    request.put("command", "add_room");

    return message_ptree(request);
}

std::string MessageClient::join_room(const int &room_id) {
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("id", room_id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return message_ptree(request);
}
