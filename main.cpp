#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

#include "client.h"
#include "server.h"


//
//CMakeFiles/mainExecutable.dir/main.cpp.o: In function `runServer()':
//main.cpp:(.text+0x5bd): undefined reference to `Server::Server(boost::asio::io_service&, short)'
//collect2: error: ld returned 1 exit status
//make[2]: *** [mainExecutable] Error 1
//make[1]: *** [CMakeFiles/mainExecutable.dir/all] Error 2
//make: *** [all] Error 2

using boost::asio::ip::tcp;


/// Returns a string version of the current day and time.
std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
};


void do_run(io_service *io) {
  io->run();
  std::cout << "io_service::run ended" <<std::endl;
}


void runClient() {
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread t(&do_run, &io_service);

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
  std::thread thread1( runServer );

  //std::chrono::milliseconds timespan(3000);
  //std::this_thread::sleep_for(timespan);

  std::thread thread2( runClient );

  thread1.join();
  thread2.join();
  return 0;
}
