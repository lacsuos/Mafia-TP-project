#pragma once

#include <string>
#include <string_view>
#include <boost/property_tree/ptree.hpp>

constexpr std::string_view MSG_END = "\n\r\n\r";

class Message {
public:
    virtual ~Message() = 0;

protected:
    static std::string message_ptree(boost::property_tree::ptree const &request);
};

class MessageClient : public Message {
public:
    static std::string ping();

    static std::string disconnect();

    static std::string create_room();

    static std::string start_game();

    static std::string join_room(const int &room_id);
};

class MessageServer : public Message {
public:
    static std::string day();

    static std::string nigth();

    static std::string vote(const int &voted_id);

    static std::string vote_mafia(const int &voted_id);
};