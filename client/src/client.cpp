#include "client.h"
#include "messages.h"

#include <string_view>
#include <iostream>

#include <boost/thread/thread.hpp>

constexpr std::string_view SERVER_IP = "127.0.0.1";
constexpr uint32_t PING = 500;
constexpr size_t SERVER_PORT = 5000;
using boost::asio::ip::address;

namespace net{
Client::~Client(){
    socket.close();
}

bool Client::connect(){
    tcp::endpoint endp(address::from_string(std::string(SERVER_IP)), SERVER_PORT);
    boost::system::error_code error;
    socket.connect(endp, error);
    if(error){
        std::cout << error.message() << std::endl;
        return false;
    }
    isClosing.store(false);
    std::cout << "connection done on ep = " << endp << std::endl;
    lastPing = boost::posix_time::microsec_clock::local_time();
    return true;
}

void Client::disconnect(){}
bool Client::isConnected() { return socket.is_open(); }

void Client::run(){
    while (!isClosing.load()){
        if (requestQ.isEmpty()){
            auto latency = boost::posix_time::microsec_clock::local_time() - lastPing;
            if (latency.total_milliseconds() < PING){
                continue;
        }
            out << Message::msg();
        }
        else {
            out << requestQ.pop();
        }
        boost::asio::write(socket, writeBuffer);
        boost::asio::read_until(socket, readBuffer, std::string(MSG_END));
        std::string answer(std::istreambuf_iterator<char>(in), {});
        answerQ.push(answer);
        lastPing = boost::posix_time::microsec_clock::local_time();
    }
}

void Client::gameVote(){}
void Client::gameAction(){}

std::string Client::getLastMsg(){
    while(answerQ.isEmpty()){
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
    return answerQ.pop();
}
}