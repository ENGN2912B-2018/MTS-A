#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

#include "image/image.h"
#include "client/client.h"
#include "server/server.h"

using namespace client;
using namespace server;

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


int main()
{

  // std::thread thread1(runServer);
  // std::thread thread2(runClient);

  // thread1.join();
  // thread2.join();

  // binary vector for Huffman encoded coefficients to send over to the server.
  std::vector< std::vector<bool> > HuffmanVec;
  // read a pgm file in given directory, second argument is binary flag(true for bianry pgm and false otherwise).
  Image testImage1("../images/dog.binary.pgm", true);
  // compress performs both dct and quantization.
  testImage1.compress(90);
  // zigzag scan to get the cofficients block by block, then Huffman encode all coefficeints in the image.
  HuffmanVec = testImage1.HuffmanEncode();

  /*
    put the codes for sending the bool vectors over the network here.
  */

  // decoded the binary vectors back to coefficients and unpack them into the coefficient image in zigzag fashion.
  testImage1.HuffmanDecode(HuffmanVec);
  // performs inverse dct.
  testImage1.decompress();
  // save the decompressed image in given directory as binary or ascii file(true for binary).
  testImage1.saveImage("../images/dog/dog90.binary.pgm", true);;
}
