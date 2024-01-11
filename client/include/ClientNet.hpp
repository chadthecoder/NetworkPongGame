#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>

#include <iostream>

struct SEND_REC_STRUCT
{
        int sendAns;
        int recAns;
        std::string recString;
};

struct REC_STRUCT
{
        int recAns;
        std::string recString;
};

inline void handle_receive()
{
    return;
}

class ClientNet
{
public:
    ClientNet(std::string ip, std::string port);
    ~ClientNet();

    SEND_REC_STRUCT SendAndRecMessage(std::string mess);

private:
    int SendMessage(std::string mess);
    REC_STRUCT RecMessage();

    asio::io_context io_context;
    asio::ip::udp::socket socket_;
    std::array<char, 128> send_buf;
    asio::ip::udp::endpoint receiver_endpoint;
    std::array<char, 128> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    size_t len;

    std::string ip;
    std::string port;
};