
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>


using boost::asio::ip::tcp;

/*
  Nomenclature:
    1. Endpoints: nodes on the network where we can create and connect sockets at.
*/

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
};

void buildClient()
{
  try
{
  std::cout << "Initializing client ..." << std::endl;
  boost::asio::io_service io_service;

  // use the tcp resolver to obtain the host name.
  tcp::resolver resolver(io_service);
  std::string hostName = boost::asio::ip::host_name();
  std::cout << "Host name is: " << hostName << std::endl;
  // port number here should be a string not int!
  tcp::resolver::query query( hostName, "2016" );
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  // after we have resolved the query string, we can create a socket and connects it to the endpoint.
  tcp::socket socket(io_service);
  boost::asio::connect(socket, endpoint_iterator);

  for (;;)
  {
    boost::array<char, 128> buf;
    boost::system::error_code error;
    // keep reading the 
    size_t len = socket.read_some( boost::asio::buffer(buf), error );

    if (error == boost::asio::error::eof)
      break; // Connection closed cleanly by peer.
    else if (error)
      throw boost::system::system_error(error); // Some other error.

    std::cout.write(buf.data(), len);
  }
}
catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
}

};

void buildServer(short port)
{
  try
  {
    std::cout << "Initializing tcp server on port " << port << " ..."<< std::endl;

    boost::asio::io_service io_service;
    tcp::acceptor acceptor( io_service, tcp::endpoint(tcp::v4(), port) );

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      std::string message = make_daytime_string();

      boost::system::error_code ignored_error;
      boost::asio::write( socket, boost::asio::buffer(message), ignored_error );
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

};

int main()
{
  std::thread thread1( buildServer, 2016 );
  std::thread thread2( buildClient );

  thread1.join();
  thread2.join();

  return 0;
}
