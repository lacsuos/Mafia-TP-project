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
    boost::property_tree::ptree request;

    request.put("command", "message_from_server");

    return message_phtee(request);
}

std::string Message::disconnect() {
    boost::property_tree::ptree request;

    request.put("command", "disconnected from server");

    return message_phtee(request);
}

std::string Message::create_room() {
    boost::property_tree::ptree request;

    request.put("command", "disconnected from server");

    return message_phtee(request);
}