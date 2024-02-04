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

      std::string messageStr = *message;

          //check if client is already connected
            //clients.size();
            unsigned short port = remote_endpoint_.port();

            bool clientIsInClients = false;
            auto pos1 = clients.find(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":1");
            auto pos2 = clients.find(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":2");
 
            if (pos1 == clients.end() && pos2 == clients.end()) clientIsInClients = false;
            else clientIsInClients = true;

            if(clients.size() == 0 && !clientIsInClients)
            {
              clients.insert(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":1");
              //messageStr += remote_endpoint_.address().to_string()+":"+std::to_string(port)+":1";
              messageStr.append(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":1");
              //messageStr.append(remote_endpoint_.address().to_string());
              //messageStr.append(":");
              //messageStr.append(std::to_string(port));
              //messageStr.append(":1");
            }
            else if(clients.size() == 1 && !clientIsInClients)
            {
              clients.insert(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":2");
              messageStr.append(remote_endpoint_.address().to_string()+":"+std::to_string(port)+":2");
            }
            else if(clients.size() >= 2 && !clientIsInClients)
            {
              std::cout << "You can not be added because there are already 2 people playing Pong.\n";
              //exit(0);
              //tell client that there is no more room and do not add
              socket_.async_send_to(asio::buffer("noroom"), remote_endpoint_,
                std::bind(&UAS::handle_send,
                this,
                message,
                std::placeholders::_1,
                std::placeholders::_2));

                //return;//right one?
            }
            else
            {
              std::cout << "Something is wrong with the size of clients or clientIsInClients.\n";
            }

            //std::cout << "creating session on: " 
            //        << remote_endpoint_.address().to_string() 
            //        << ":" << remote_endpoint_.port() << "\n";// << str << '\n';

            //add clients to send string and cout the clients

            std::cout << "Current clients are: "; 
            std::set<std::string>::iterator itr;
            for (itr = clients.begin(); itr != clients.end(); itr++)
            {
              std::cout << *itr << ", ";
            }        
            std::cout << "\n";

      //socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
      //    std::bind(&UAS::handle_send, this, message,
      //      std::placeholders::_1,
      //      std::placeholders::_2));

      socket_.async_send_to(asio::buffer(messageStr), remote_endpoint_,
          std::bind(&UAS::handle_send, this, message,
            std::placeholders::_1,
            std::placeholders::_2));

      start_receive();
  }
}

void UAS::handle_send(std::shared_ptr<std::string> /*message*/,
      const asio::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
{}