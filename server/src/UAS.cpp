#include <UAS.hpp>

UAS::UAS(asio::io_context& io_context, asio::ip::port_type port_num)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 1025))
{
  start_receive();
}

std::string UAS::make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

void UAS::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UAS::handle_receive,
        this,
        std::placeholders::_1,
        std::placeholders::_2));
}

void UAS::handle_receive(const asio::error_code& error,
  std::size_t /*bytes_transferred*/)
{
    if (!error)
    {
      std::cout << "rec from client: " << recv_buffer_ .data() << "\n";

      std::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

      socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
          std::bind(&UAS::handle_send, this, message,
            std::placeholders::_1,
            std::placeholders::_2));

            std::cout << "creating session on: " 
                    << remote_endpoint_.address().to_string() 
                    << ":" << remote_endpoint_.port() << "\n";// << str << '\n';

      start_receive();
  }
}

void UAS::handle_send(std::shared_ptr<std::string> /*message*/,
      const asio::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
{}