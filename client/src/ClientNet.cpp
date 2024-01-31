#include <ClientNet.hpp>

ClientNet::ClientNet(std::string ip, std::string rec_port) : socket_(this->io_context), ip(ip), rec_port(rec_port), resolver(this->io_context)
{
    this->socket_.open(asio::ip::udp::v4());
}

ClientNet::~ClientNet()
{
}

SEND_REC_STRUCT ClientNet::SendAndRecMessage(std::string mess)
{
    SEND_REC_STRUCT ans;
    ans.sendAns = this->SendMessage(mess);
    REC_STRUCT rec = this->RecMessage();
    ans.recAns = rec.recAns;
    ans.recString = rec.recString;

    return ans;
}

int ClientNet::SendMessage(std::string mess)
{
    std::shared_ptr<std::string> messPtr(
          new std::string(mess));

    //asio::ip::udp::resolver resolver(this->io_context);
    //std::cout << "send mess ip: " << this->ip << " and port: " << this->port << "\n";
    this->receiver_endpoint =
      *resolver.resolve(asio::ip::udp::v4(), this->ip, this->rec_port).begin();

    // udp::socket socketOne(io_context);
    //this->socket_.open(asio::ip::udp::v4());

    // boost::array<char, 1> send_buf = {{0}};
    this->socket_.send_to(asio::buffer(*messPtr), this->receiver_endpoint);
    //this->socket_.send_to(asio::buffer(this->send_buf), this->receiver_endpoint);
    //std::cout << "128 buff sennt to server" << std::endl;

    return 0; 
}

REC_STRUCT ClientNet::RecMessage()
{
    REC_STRUCT ans;

    //std::cout << "In startreceive\n"
    //        << std::endl;

    //this->sender_endpoint =
    //  *resolver.resolve(asio::ip::udp::v4(), this->ip, this->rec_port).begin();
    //this->sender_endpoint =
    //  *resolver.resolve(asio::ip::udp::v4(), this->ip, "1028").begin();

    this->len = socket_.receive_from(asio::buffer(this->recv_buf), this->sender_endpoint);
    /*socket_.async_receive_from(
            asio::buffer(this->recv_buf), this->sender_endpoint,
            std::bind(handle_receive));*/

    //std::cout << "lh[dshlfihdsilfhl;ds\n"
    //        << len << "\n"
    //        << std::endl;

    //std::cout.write(recv_buf.data(), len);

    ans.recAns = 0;
    ans.recString = recv_buf.data();

    if(ans.recString == "noroom")
    {
        std::cout << "No room for you on server!\n";
        //exit(0);
        abort(); 
    }

    //std::cout << "start receive test: end of rec\n";

    return ans;
}