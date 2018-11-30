#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

#include "image.h"
#include "client/client.h"
#include "server/server.h"

using namespace image;
using namespace client;
using namespace server;

using std::vector;
using boost::asio::ip::tcp;

void runClient() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);

    // seperate thread for running io_service, so this thread can send messages
    std::thread t([&io_service](){ io_service.run(); });

    // use the tcp resolver to obtain the host name.
    tcp::resolver resolver(io_service);
    std::string hostName = boost::asio::ip::host_name();

    // port number here should be a string not int
    tcp::resolver::query query(hostName, "8008");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    std::cout << "about to create client" << std::endl;
    Client c(io_service, endpoint_iterator);
    c.write("Hello World");
    c.write("How are you?");
    c.write("Why don't I get a response?");

    // send image over (1352 bytes to send)
    //vector<vector<unsigned>> img = readImage("../images/feep.pgm");
    //vector<vector<double>> coeff = toCoefficients(img);
    //vector<char> ser = serialize(coeff);
    //c.write(ser.data());
    t.join();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void runServer() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );
    Server s(io_service, 8008);
    io_service.run();
  } catch (std::exception e) {
   std::cerr << e.what() << std::endl;
  }

}


/// test serialization/deserialization works
void testImage() {
  vector<vector<unsigned>> img = readImage("../images/feep.pgm");
  vector<vector<double>> coeff = toCoefficients(img);

  assert(img.size() == 7);
  assert(img[0].size() == 24);
  assert(coeff.size() == 7);
  assert(coeff[0].size() == 24);

  vector<char> ser = serialize(coeff);
  vector<vector<double>> copy = deserialize(ser);

  std::cout << ser.size() << std::endl;
  assert(copy.size() == 7);
  assert(copy[0].size() == 24);

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 24; j++) {
      assert(coeff[i][j] == copy[i][j]);
    }
    std::cout << std::endl;
  }
}



int main() {
  std::thread thread1(runServer);
  std::thread thread2(runClient);

  thread1.join();
  thread2.join();
  return 0;
}
