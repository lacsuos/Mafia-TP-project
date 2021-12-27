#include "client.h"
#include "messages.h"

#include <string_view>
#include <iostream>
#include <boost/thread/thread.hpp>

constexpr std::string_view SERVER_IP = "127.0.0.1";
constexpr uint32_t PING = 500;
constexpr size_t SERVER_PORT = 5000;
using boost::asio::ip::address;

namespace net {
    Client::~Client() {
        socket.close();
    }

    bool Client::connect() {
        boost::asio::ip::tcp::endpoint endp(address::from_string(std::string(SERVER_IP)),
                                            SERVER_PORT);
        boost::system::error_code error;
        socket.connect(endp, error);
        if (error) {
            std::cout << error.message() << std::endl;
            return false;
        }
        isClosing.store(false);
        std::cout << "connection done on ep = " << endp << std::endl;
        lastPing = boost::posix_time::microsec_clock::local_time();
        return true;
    }

    bool Client::disconnect() {
        auto msg = MessageClient::disconnect();
        requestQ.Push(msg);
        isClosing.store(true);
        return true;
    }

    bool Client::isConnected() {
        return socket.is_open();
    }

    void Client::run() {
        while (!isClosing.load()) {
            if (requestQ.IsEmpty()) {
                auto latency = boost::posix_time::microsec_clock::local_time() - lastPing;
                if (latency.total_milliseconds() < PING) {
                    continue;
                }
                out << MessageClient::ping();
            } else {
                out << requestQ.Pop();
            }
            boost::asio::write(socket, writeBuffer);

            boost::asio::read_until(socket, readBuffer, std::string(MSG_END));

            std::string answer(std::istreambuf_iterator<char>(in), {});
            answerQ.Push(answer);
            lastPing = boost::posix_time::microsec_clock::local_time();
        }
        while (!requestQ.IsEmpty()) {
            out << requestQ.Pop();

            boost::asio::write(socket, writeBuffer);

            boost::asio::read_until(socket, readBuffer, std::string(MSG_END));

            std::string answer(std::istreambuf_iterator<char>(in), {});
            answerQ.Push(answer);

            lastPing = boost::posix_time::microsec_clock::local_time();
        }

        socket.close();
    }

    std::string Client::getLastMsg() {
        while (answerQ.IsEmpty()) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }
        return answerQ.Pop();
    }

    void Client::startGame() {
        auto msg = MessageClient::create_room();
        requestQ.Push(msg);
    }

    void Client::joinGame(const int &room_id) {
        auto msg = MessageClient::join_room(room_id);
        requestQ.Push(msg);
    }
}
