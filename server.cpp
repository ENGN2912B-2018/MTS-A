#include <string>
#include <thread>
#include <cassert>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

#include "image/image.h"
#include "server/server.h"

using namespace server;

using std::vector;
using boost::asio::ip::tcp;

/// The main server executable.
int main() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );
    Server s(io_service, 8008);
    io_service.run();
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
