//TCP Async Server Connection Class
#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>

#include <memory>

#include <utils/utils.cpp>

class TAScon
  : public std::enable_shared_from_this<TAScon>
{
public:
  typedef std::shared_ptr<TAScon> pointer;

  static pointer create(asio::io_context& io_context)
  {
    return pointer(new TAScon(io_context));
  }

  asio::ip::tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    asio::async_write(socket_, asio::buffer(message_),
        std::bind(&TAScon::handle_write, shared_from_this()));
  }

private:
  TAScon(asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write()
  {
  }

  asio::ip::tcp::socket socket_;
  std::string message_;
};