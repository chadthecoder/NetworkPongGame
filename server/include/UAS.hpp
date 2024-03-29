//UDP Async Server
#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>

#include <iostream>
//#include <set>
//#include <chrono>
//#include <vector>

#include <utils.hpp>
#include <Client.hpp>

class UAS
{
public:
  UAS(asio::io_context& io_context, asio::ip::port_type port_num);
  void start_receive();

private:
  void handle_receive(const asio::error_code& error,
      std::size_t /*bytes_transferred*/);

  void handle_send(std::shared_ptr<std::string> /*message*/,
      const asio::error_code& /*error*/,
      std::size_t /*bytes_transferred*/);

  asio::ip::udp::socket socket_;
  asio::ip::udp::endpoint remote_endpoint_;
  std::array<char, 128> recv_buffer_;

  std::chrono::_V2::steady_clock::time_point time_last, time_now;
  std::chrono::milliseconds time_diff;

  double timeDiffAnsDouble;

  //std::set<std::string> clients;
  std::vector<std::string> clients;
  std::vector<std::chrono::_V2::steady_clock::time_point> clients_time_last, clients_time_now;
  std::vector<std::chrono::milliseconds> clients_time;
};