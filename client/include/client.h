#pragma once
#include "myqueue.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic/atomic.hpp>
#include <iostream>
#include <queue>



using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net{
class Client{
private:
    io_context context;
    tcp::socket socket;

    boost::asio::streambuf writeBuffer;
    boost::asio::streambuf readBuffer;

    std::ostream out;
    std::istream in;
    std::queue<std::string> messageq;
    boost::posix_time::ptime lastPing;
    boost::atomic<bool> isClosing;
    ThreadQueue<std::string> requestQ;
    ThreadQueue<std::string> answerQ;

    

public:
    Client(): socket(context), out(&writeBuffer), in(&readBuffer), isClosing(false) {};
    Client(Client &some) = delete;
    ~Client();
    void operator=(const Client &) = delete;

    void run();
    bool connect();
    void disconnect();
    bool isConnected();
    void gameVote();
    void gameAction();
    std::string getLastMsg();
};

} //namespace net