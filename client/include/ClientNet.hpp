#define ASIO_STANDALONE
#include <asio.hpp>

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

class ClientNet
{
public:
    ClientNet(/* args */);
    ~ClientNet();

    SEND_REC_STRUCT SendAndRec(std::string);

private:
    int StartSend(std::string);
    REC_STRUCT StartReceive();

    asio::io_context io_context;
    std::string ip;
    asio::ip::udp::socket socket_;
    std::array<char, 128> send_buf;
    asio::ip::udp::endpoint receiver_endpoint;
    std::array<char, 128> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    size_t len;
    std::string port;
};