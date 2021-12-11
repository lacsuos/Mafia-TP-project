#pragma once

#include <string>
#include <string_view>
#include <boost/property_tree/ptree.hpp>

constexpr std::string_view MSG_END = "\n\r\n\r";

class Message {
public:
    virtual ~Message() = 0;

    static std::string message_phtee(boost::property_tree::ptree const &request);
};

class MessageClient : public Message {
public:
    static std::string msg();
    static std::string disconnect();
    static std::string create_room();
    static std::string error();
    static std::string join_room(const size_t& room_id);

};

class MessageServer : public Message {
public:
    static std::string start_game(const size_t &id);
    static std::string start_game_failed();
    static std::string create_room_failed();
    static std::string create_room_done(const int &id);
    static std::string connected(const std::vector<std::vector<std::string>> &users_ip, const int &role);
//    static std::string game_start(const int role);
    static std::string accept_room_done(const size_t& room_id);
    static std::string join_room_failed(const int &id);
    static std::string leave_room_done();
};