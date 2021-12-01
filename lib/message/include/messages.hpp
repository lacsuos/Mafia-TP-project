#pragma once

#include <string>
#include <string_view>

#include <boost/property_tree/ptree.hpp>

constexpr std::string_view MSG_END = "\n\r\n\r";

class Message {
public:
    static std::string message_phtee(boost::property_tree::ptree const &request);
    static std::string msg();
    static std::string disconnect();
    static std::string create_room();
};