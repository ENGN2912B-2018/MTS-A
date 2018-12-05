#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

#include "image/image2.h"
#include "client/client.h"
#include "server/server.h"

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

int main() {
  // std::thread thread1(runServer);
  // std::thread thread2(runClient);
  //
  // thread1.join();
  // thread2.join();

  Image testImage("../mirrordog.pgm");

  // testImage.compress(0);
  // testImage.decompress();

  testImage.saveImage("../mirrordog1.pgm");

  // std::vector< std::vector<double> > vec1 = testImage.intMatrix_;
  // std::vector< std::vector<double> > vec2 = testImage.coefMatrix_;
  //
  // for(int i=0; i<16; i++)
  // {
  //   for(int j=0; j<16; j++)
  //   {
  //     std::cout << vec1[i][j] << "  ";
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << "\n\n";
  // for(int i=0; i<16; i++)
  // {
  //   for(int j=0; j<16; j++)
  //   {
  //     std::cout << vec2[i][j] << "  ";
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << "\n";
  // std::vector<int> vec1 = {4,1,2,0,3,5,8};
  // std::vector<int> vec2 = heapify(vec1);
  // for(int i=0; i<vec2.size(); i++){ std::cout << vec2[i] << "  "; }
  //
  // Image testImage("../noisyImage.pgm");
  //
  // double* uncompressedCoef = testImage.compress(0, 8);
  // std::cout << "\nUncompressed coefficients:" << std::endl;
  // for(int i=0; i<20; i++){ std::cout << uncompressedCoef[i] << " ";}
  //
  // double* compressedCoef = testImage.compress(10, 8);
  // std::cout << "\n\nCompressed coefficients:" << std::endl;
  // for(int i=0; i<20; i++){ std::cout << compressedCoef[i] << " ";}
  //
  // Image coefImage = testImage.compress();
  //
  // std::vector<double> uncompressedIntensities = coefImage.decompress(uncompressedCoef, 8);
  // std::vector<double> compressedIntensities = coefImage.decompress(compressedCoef, 8);
  //
  // std::cout << "\n\nUncompressed Intensities... " << std::endl;
  // for(int i=0; i<uncompressedIntensities.size(); i++){ std::cout << uncompressedIntensities[i] << " ";}
  // std::cout << "\n\nCompressed Intensities... " << std::endl;
  // for(int i=0; i<compressedIntensities.size(); i++){ std::cout << compressedIntensities[i] << " ";}
  // std::cout << "\n" << std::endl;
  //
  // delete[] uncompressedCoef;
  // delete[] compressedCoef;
}
