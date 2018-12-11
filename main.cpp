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


std::vector< std::vector<int> > padding(std::vector<int> coefVec)
{
  std::vector<int> coefVector;
  std::vector< std::vector<int> > coefMatrix;

  for(int i=0; i<coefVec.size(); i++)
  {
    if(coefVec[i] == 2220)
    {
      // resize a vector initializes the empty elements to 0 anyway.
      coefVector.resize(64);
      coefMatrix.push_back(coefVector);
      coefVector.clear();
    }
    else{ coefVector.push_back(coefVec[i]); }

  }

  return coefMatrix;
};


std::vector< std::vector<int> > recoverCoefMatrix()
{
  int i, j;
  int hBlockCount, startingRow, startingColumn;
  std::vector< std::vector<int> > coefMatrix;

  hBlockCount = fileColumns_/8;

  coefMatrix.resize(fileRows_);
  for(i=0; i< coefMatrix.size(); i++){ coefMatrix[i].resize(fileColumns_); }

  for(int i=0; i<coefMatrix2.size(); i+=)
  {

    startingRow = i / hBlockCount;
    start_column = (i % hBlockCount) * 8;

    for(i = startingRow; i < startingRow + blockSize_; i++)
    {
      for(j = startingColumn; j < startingColumn + blockSize_; j++)
      {
        coefMatrix[i][j] = ;
      }
    }

  }

  return coefMatrix;
}



int main()
{

  // std::thread thread1(runServer);
  // std::thread thread2(runClient);

  // thread1.join();
  // thread2.join();

  // Image testImage("../images/pepper.ascii.pgm", false);

  // testImage.sequentialCompression("../images/pepper/", true);

  // statisticalAnalysis stats;

  HuffmanCoding Huffman;

  std::vector<int> coefVec;
  std::vector< std::vector<bool> > HuffmanVec;

  Image testImage1("../images/dog.binary.pgm", true);
  // Compress performs both dct and quantization.
  testImage1.compress(50);
  // Zigzag scan creates a vector of coefficients scanned in zigzag fashion block by block(integer 2220 signals end of block).
  coefVec = testImage1.zigzagScan();
  // Build the Huffman tree from all coefficients and generate a vector that contains
  // the Huffman coded coefficients for the whole image.
  HuffmanVec = Huffman.encode(coefVec);

  /*
    put the codes for sending the bool vectors over the network here.
  */

  std::vector<int> coefVec2 = Huffman.decode(HuffmanVec);
  std::vector< std::vector<int> > coefMatrix2 = padding(coefVec2);

  // for(int i=0; i<20; i++){ std::cout << coefMatrix2[0][i] << " "; }
  // testImage1.decompress();

  // testImage1.saveImage("../images/dog/dog70.binary.pgm", true);

  // int mse = stats.MSE(testImage1.intMatrix_, testImage1.compIntMatrix_);
  // std::cout << "\nmse: " << mse << std::endl;

  // double psnr = stats.PSNR(255, mse);
  // std::cout << "psnr: " << psnr << std::endl;
}
