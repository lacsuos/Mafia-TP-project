#include "messages.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;

std::string Message::messagePtree(boost::property_tree::ptree const &request) {
    std::stringstream json_request;
    pt::write_json(json_request, request);
    return json_request.str() + "\r\n\r";
}

std::string Message::msg() {
    boost::property_tree::ptree request;

    request.put("command_type", "ping");
    request.put("command", "ping");

    return messagePtree(request);
}

std::string Message::joinRoom(const int &id){
    pt::ptree parametrs;
    pt::ptree request;

    parametrs.put("status", "on");
    parametrs.put("id", id);

    request.put("command_type", "basic");
    request.put("command", "join_room");
    request.add_child("parametrs", parametrs);

    return messagePtree(request);
}

std::string Message::createRoom(){
    pt::ptree parametrs;
    pt::ptree request;


    request.put("command_type", "basic");
    request.put("command", "add_room");
    request.add_child("parametrs", parametrs);
    return messagePtree(request);
}