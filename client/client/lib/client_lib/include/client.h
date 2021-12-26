#pragma once

#include "myqueue.h"
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic/atomic.hpp>
#include <iostream>
#include <queue>

namespace net {
    class Client {
    private:
        boost::asio::io_context context;
        boost::asio::ip::tcp::socket socket;

        boost::asio::streambuf writeBuffer;
        boost::asio::streambuf readBuffer;

        std::ostream out;
        std::istream in;

        boost::posix_time::ptime lastPing;
        boost::atomic<bool> isClosing;

        ThreadQueue<std::string> requestQ;
        ThreadQueue<std::string> answerQ;


    public:
        Client() : socket(context), out(&writeBuffer), in(&readBuffer), isClosing(false) {};

        Client(const Client &some) = delete;

        ~Client();

        void operator=(const Client &) = delete;

        void run();

        bool connect();

        bool disconnect();

        bool isConnected();

        void startGame();

        void joinGame(const int &room_id);

        std::string getLastMsg();
    };

} //namespace net
