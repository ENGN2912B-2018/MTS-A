#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

#include "client.h"
#include "server.h"

using boost::asio::ip::tcp;

/*
Nomenclature:
  1. Endpoints: nodes on the network where we can create and connect sockets at.
*/

//about to create client
//in client do_connect
//in client write
//in client write
//in write callback
//in client do_write
//server accpected client!
//in write callback
//in session start
//in session do_read
//in session do_read callback, length of message: client connected successfully!
//in write callback
//in client do_write
//11   strlen of data_: 11
//in session do_write
//in write callback
//in session do_write callback
//in session do_read
//in session do_read callback, length of message: 12   strlen of data_: 12
//in session do_write
//in session do_write callback
//in session do_read


/// Returns a string version of the current day and time.
std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
};


void do_run(io_service *io)
{
  io->run();
  std::cout << "io_service::run ended" <<std::endl;
}


void runClient() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );

    // use the tcp resolver to obtain the host name.
    tcp::resolver resolver(io_service);
    std::string hostName = boost::asio::ip::host_name();

    // port number here should be a string not int
    tcp::resolver::query query( hostName, "2016" );
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    std::cout << "about to create client" << std::endl;
    Client c(io_service, endpoint_iterator);
    c.write("Hello World");
    c.write("How are you?");
    c.write("Why don't I get a response?");
    std::thread t(&do_run, &io_service);
    t.join();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void runServer() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );
    Server s(io_service, 2016);
    io_service.run();
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }

}



int main() {
  std::thread thread1( runServer );
  std::thread thread2( runClient );

  thread1.join();
  thread2.join();
  return 0;
}
