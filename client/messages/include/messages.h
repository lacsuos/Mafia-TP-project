#pragma once

#include <string>
#include <string_view>

#include <boost/property_tree/ptree.hpp>

constexpr std::string_view MSG_END = "\n\r\n\r";

class Message {
private:
    static std::string messagePtree(boost::property_tree::ptree const &request);
public:
    static std::string msg();
    static std::string joinRoom(const int &id);
    static std::string createRoom();
};