#include <ClientNet.hpp>

ClientNet::ClientNet(std::string ip, std::string rec_port) : socket_(this->io_context), ip(ip), rec_port(rec_port), resolver(this->io_context)
{
    this->socket_.open(asio::ip::udp::v4());
}

ClientNet::~ClientNet()
{
}

//each var is 4 chars
//1st variable: 0000==continue-game, 0001==left-wins, 0002==right-wins
//2nd variable: 0000==no-points, 0001==left-point, 0002==right-point
//3rd variable: left paddle x
//4th variable: left paddle y
//5th variable: right paddle x
//6th variable: right paddle y
//7th variable: 000-left score
//8th variable: 000-right score
//9th variable: game ball x
//10th variable: game ball y
//11th variable: game ball x velocity
//12th variable: game ball y velocity
SEND_REC_STRUCT ClientNet::SendAndRecMessage(NetworkMessage mess)
{
    std::string messString;

    utils::push4char(messString, mess.win);
    utils::push4char(messString, mess.point);
    utils::push4char(messString, mess.leftPaddleX);
    utils::push4char(messString, mess.leftPaddleY);
    utils::push4char(messString, mess.rightPaddleX);
    utils::push4char(messString, mess.rightPaddleY);
    utils::push4char(messString, mess.leftScore);
    utils::push4char(messString, mess.rightScore);
    utils::push4char(messString, mess.gameballX);
    utils::push4char(messString, mess.gameballY);
    utils::push4char(messString, mess.gameballXVel);
    utils::push4char(messString, mess.gameballYVel);

    return SendAndRecMessage(messString);
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