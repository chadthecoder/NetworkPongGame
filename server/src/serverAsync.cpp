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

#include <UAS.hpp>

#define ASIO_STANDALONE
#include <asio.hpp>

int main()
{
  /*try
  {
    asio::io_context io_context;
    UAS server(io_context, 1025);
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
      UAS server(io_context, 1025);
      //server.start_receive();
      io_context.run();
    }
    //will execute if server is already running, have logic to be able to start muultiple servers on one computer?
    catch (std::exception& e)
    {
      std::cout << "errorrrr brooo\n";
      std::cerr << e.what() << std::endl;
    }
  //}

  return 0;
}