//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
//#include <boost/array.hpp>
//#include <boost/bind/bind.hpp>
//#include <boost/shared_ptr.hpp>
#define ASIO_STANDALONE
#include <asio.hpp>

//using asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class udp_server
{
public:
  udp_server(asio::io_context& io_context, asio::ip::port_type port_num)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 1025))
  {
    start_receive();
  }

  void start_receive()
  {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&udp_server::handle_receive, this,
          std::placeholders::_1,
          std::placeholders::_2));
  }

private:
  void handle_receive(const asio::error_code& error,
      std::size_t /*bytes_transferred*/)
  {
    if (!error)
    {
      std::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

      socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
          std::bind(&udp_server::handle_send, this, message,
            std::placeholders::_1,
            std::placeholders::_2));

            char *str;
            //sprintf(str, "%u", remote_endpoint_.port());

            std::cout << "creating session on: " 
                    << remote_endpoint_.address().to_string() 
                    << ":" << remote_endpoint_.port() << ":";// << str << '\n';

      start_receive();
    }
  }

  void handle_send(std::shared_ptr<std::string> /*message*/,
      const asio::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }

  asio::ip::udp::socket socket_;
  asio::ip::udp::endpoint remote_endpoint_;
  std::array<char, 128> recv_buffer_;
};

int main()
{
  /*try
  {
    asio::io_context io_context;
    udp_server server(io_context, 1025);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cout << "errorrrr brooo\n";
    std::cerr << e.what() << std::endl;
  }*/

  //while(true)
  //{
    try
    {
      asio::io_context io_context;
      udp_server server(io_context, 1025);
      //server.start_receive();
      io_context.run();
    }
    catch (std::exception& e)
    {
      std::cout << "errorrrr brooo\n";
      std::cerr << e.what() << std::endl;
    }
  //}

  return 0;
}