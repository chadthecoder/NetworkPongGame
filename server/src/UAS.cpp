#include <UAS.hpp>

UAS::UAS(asio::io_context& io_context, asio::ip::port_type port_num)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 1025)),
      recTime(std::time(nullptr)), lastRecTime(std::time(nullptr)), timeDiff(std::time(nullptr)),
      tRec(clock()), tLastRec(clock()), tDiff(clock())
{
  start_receive();
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
    lastRecTime = recTime;
    recTime = std::time(nullptr);
    timeDiff = recTime-lastRecTime;

    tLastRec = tRec;
    tRec = clock();
    tDiff = tRec-tLastRec;
    timeDiffAnsDouble = difftime(tRec, tLastRec);

    std::string timeDiffStr = std::ctime(&timeDiff);
    std::cout << "Time Diff String: " << timeDiffAnsDouble << "\n";

    if (!error)
    {
      std::cout << "rec from client: " << recv_buffer_ .data() << "\n";

      std::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

      socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
          std::bind(&UAS::handle_send, this, message,
            std::placeholders::_1,
            std::placeholders::_2));

            //check if client is already connected
            unsigned short port = remote_endpoint_.port();
            clients.insert(remote_endpoint_.address().to_string()+std::to_string(port));

            //std::cout << "creating session on: " 
            //        << remote_endpoint_.address().to_string() 
            //        << ":" << remote_endpoint_.port() << "\n";// << str << '\n';

            std::cout << "Current clients are: "; 
            std::set<std::string>::iterator itr;
            for (itr = clients.begin(); itr != clients.end(); itr++)
            {
              std::cout << *itr << ", ";
            }        
            std::cout << "\n";

      start_receive();
  }
}

void UAS::handle_send(std::shared_ptr<std::string> /*message*/,
      const asio::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
{}