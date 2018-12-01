#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cmath>
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

int partition(double* array, int left, int right)
{
    int pivotIndex = left;
    double pivot = array[left];
    double temp;

    while(left <= right)
    {
        while(array[left] < pivot){ left += 1; continue; }
        while(array[right] >= pivot){ right -= 1; continue; }

        if(left == pivotIndex){ pivotIndex = right; }
        if(right == pivotIndex){ pivotIndex = left; }

        temp = array[left];
        array[left] = array[right];
        array[right] = temp;

        temp = array[left];
        array[left] = array[pivotIndex];
        array[pivotIndex] = temp;
    }

    return left;
}

void quickSort(double* array, int left, int right)
{
    if(left < right)
    {
        double boundary = partition(array, left, right);

        quickSort(array, left, boundary-1);
        quickSort(array, boundary+1, right);
    }

}

double* quickSort(double* array)
{
    int left = 0;
    int right = sizeof(array)/sizeof(array[0]);

    quickSort(array, left, right);

    return array;
}

double* flattenArray(double** array)
{
    int i, j;
    int row = sizeof(array)/sizeof(array[0]);
    int column = sizeof(array[0])/sizeof(array[0][0]);

    int k = 0;
    double* flattenedArray = new double[row*column];

    for(i=0; i<row; i++){
        for(j=0; j<column; j++){
            flattenedArray[k] = array[i][j];
            k += 1;
    }   }

    return flattenedArray;
}

class Image
{

public:
    Image(unsigned row, unsigned column)
    {
      row_=row;
      column_=column;
      data_ = new double* [row_];
      for(int i=0; i<row_; i++){ data_[i] = new double [column_]; }
      for(int i=0; i<row_; i++){ for(int j=0; j<column_; j++){ data_[i][j]=0; } }
    }

    Image(std::string const& filename)
    {
      std::ifstream inputFileStream(filename, std::ios::in);
      std::stringstream fileContent;
      std::string fileType;
      double maxIntensity_;

      fileContent << inputFileStream.rdbuf();
      fileContent >> fileType;

      //could also check file extension with boost filesystem instead.
      if(fileType != "P2"){ std::cerr << "Wrong file type, please select a pgm file instead." << std::endl; exit(0); }
      fileContent >> column_ >> row_;
      fileContent >> maxIntensity_;

      std::cout << "File type: " << fileType << std::endl;
      std::cout << "Column: " << column_ << "  Row: "<< row_ << std::endl;
      std::cout << "Max Intensity: "<< maxIntensity_ << std::endl;
      std::cout << "\nReading File...." << std::endl;

      data_ = new double* [row_];
      for(int i=0; i<row_; i++){ data_[i] = new double [column_]; }
      for(int i=0; i<row_; i++){ for(int j=0; j<column_; j++){ fileContent >> data_[i][j]; std::cout << data_[i][j] << " "; } }

      inputFileStream.close();
      std::cout << "\nDone." << std::endl;
    }

    ~Image(){ for(int i=0; i<row_; i++){ delete [] data_[i]; } delete [] data_;}

    Image compress(unsigned int compressionRatio)
    {
        Image coefImage = dct();
        return coefImage;
    }

    Image decompress()
    {
        Image pixelImage = idct();
        return pixelImage;
    }

    double** data_;

private:

    Image dct()
    {
      Image coefImage(row_, column_);
      int i, j, m, n;
      double c_i, c_j, sum;

      for(i=0; i<row_; i++){
        for(j=0; j<column_; j++){

            sum = 0.0;

            if(i==0){ c_i = std::sqrt(1.0/2); }
            else{ c_i = std::sqrt(1.0/2); }

            if(j==0){ c_j = std::sqrt(1.0/2); }
            else{ c_j = std::sqrt(1.0/2); }

            for(m=0; m<row_; m++){
              for(n=0; n<column_; n++){
                  sum += data_[m][n] * std::cos( (2 * m + 1) * i * pi / (2 * row_) ) * std::cos( (2 * n + 1) * j * pi / (2 * column_) );
            } }

            coefImage.data_[i][j] = c_i * c_j * sum / std::sqrt(2 * row_);
            std::cout << "Pixel Intensity: " << data_[i][j] << " Pixel Coefficient: " << coefImage.data_[i][j] << std::endl;
      } }

      return coefImage;
    }

    Image idct()
    {
      Image pixelImage(row_, column_);
      int i, j, m, n;
      double c_m, c_n, sum;

      for(i=0; i<row_; i++){
        for(j=0; j<column_; j++){

            sum = 0.0;

            for(m=0; m<row_; m++){
              for(n=0; n<column_; n++){

                  if(m==0){ c_m = std::sqrt(1.0/2); }
                  else{ c_m = std::sqrt(1.0/2); }

                  if(n==0){ c_n = std::sqrt(1.0/2); }
                  else{ c_n = std::sqrt(1.0/2); }

                  sum += data_[m][n] * std::cos( (2 * i + 1) * m * pi / (2 * row_) ) * std::cos( (2 * j + 1) * n * pi / (2 * column_) );
            } }

            pixelImage.data_[i][j] = c_m * c_n * sum / std::sqrt(2 * row_);
            std::cout << "Pixel Coefficient: " << data_[i][j] << " Pixel Intensity: " << pixelImage.data_[i][j] << std::endl;
        }
      }

      return pixelImage;
    }

    double pi = 3.14159265358979324;
    unsigned row_;
    unsigned column_;

};


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
  // std::thread thread1(runServer);
  // std::thread thread2(runClient);
  //
  // thread1.join();
  // thread2.join();

  Image testImage("../noisyImage.pgm");

  Image coefImage = testImage.compress(10);
  std::cout << coefImage.data_[0][0]<< std::endl;

  Image pixelImage = coefImage.decompress();
  std::cout << pixelImage.data_[0][0]<< std::endl;

  return 0;
}
