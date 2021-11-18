#pragma once
#include "user.hpp"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::io_context;


class Room{
public:
    void Step();
    void addUser(std::shared_ptr<net::User> &user);

    io_context context_;
    std::vector<std::shared_ptr<net::User>> &user;
};
