#include <cstdio>
#include <cstdlib>
#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>

#include "Pong.hpp"

//using boost::asio::ip::udp;

int main(int argc, char *argv[])
{

    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: main <host>" << std::endl;
            return 1;
        }

        // boost::asio::io_context io_context1;

        // udp::resolver resolver(io_context1);
        // udp::endpoint receiver_endpoint =
        //     *resolver.resolve(udp::v4(), argv[1], "pong").begin();

        // udp::socket socketOne(io_context);
        // socketOne.open(udp::v4());

        // boost::array<char, 1> send_buf = {{0}};
        // socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        // boost::array<char, 128> recv_buf;
        // udp::endpoint sender_endpoint;
        // size_t len = socket.receive_from(
        // boost::asio::buffer(recv_buf), sender_endpoint);

        // std::cout.write(recv_buf.data(), len);

        // char *ip[];
        //  std::string ip = argv[1];
        //  char ip[9]; // assign arraythree with arraytwo
        //  for (int i = 0; i < 9; ++i)
        //{
        //  arraythree[i] = arraytwo[i];
        // }

        std::string ip = argv[1];
        std::cout << "arg: " << argv[1] << "\nip: " << ip << std::endl;

        Pong game(ip);
        std::cout << "Game init done\n"
                  << std::endl;
        bool success = game.Initialize();
        std::cout << std::to_string(success) << std::endl;
        if (success)
        {
            game.RunLoop();
        }
        game.Shutdown();
    }

    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
