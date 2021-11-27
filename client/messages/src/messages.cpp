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

    request.put("command", "message");

    return messagePtree(request);
}