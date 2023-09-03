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
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <asio.hpp>

using asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class udp_server
{
public:
    udp_server(asio::io_context &io_context)
        : socket_(io_context, udp::endpoint(udp::v4(), 1024))
    {
        start_receive();
    }

private:
    // start waiting for client to send
    void start_receive()
    {
        // other option to pass instead of asio::use_future
        //  asio::yield_context yield;

        // if implements this instead must call handle_receive separately, can do for send as well
        // std::future<std::size_t> my_future = socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_, asio::use_future);
        // handle_receive(iError1, recvSize);

        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&udp_server::handle_receive, this,
                        iError1,
                        recvSize));
    }

    // to do after recieved from client, send info back
    void handle_receive(const asio::error_code &error,
                        std::size_t /*bytes_transferred*/)
    {
        std::cout << "handle_receive" << std::endl;

        if (!error)
        {
            day = make_daytime_string();
            sizeDay = day.size();
            boost::shared_ptr<std::string> message(
                new std::string(day));

            socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                                  boost::bind(&udp_server::handle_send, this, message,
                                              iError2,
                                              sizeDay));

            // start listening for another client send
            start_receive();
        }
    }

    // what to do with info recieved?
    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const asio::error_code & /*error*/,
                     std::size_t /*bytes_transferred*/)
    {
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recv_buffer_;
    std::string day;
    std::size_t recvSize = recv_buffer_.size(), sizeDay;
    boost::system::error_code iError1, iError2;
};

int main()
{
    try
    {
        asio::io_context io_context;
        udp_server server(io_context);
        io_context.run();
        // asio::steady_timer t(io_context, asio::chrono::seconds(5));
        //  t.wait();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}