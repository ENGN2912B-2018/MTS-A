#include <string>
#include <omp.h>
#include <thread>
#include <cassert>
#include <iostream>
#include <boost/asio.hpp>
#include <QApplication>

#include "client/client.h"
#include "gui/mainwindow.h"
#include "image/image.h"
#include "server/server.h"

using namespace gui;
using namespace client;
using namespace server;

using std::vector;
using boost::asio::ip::tcp;


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

int main(int argc, char* argv[]) {
  // try running the server and client
  try {
    // run the server
    boost::asio::io_service server_io;
    boost::asio::io_service::work server_work(server_io);
    Server s(server_io, 8008);

    std::thread server([&server_io](){
      server_io.run();
    });
    server.detach();


    // run the client
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
    t.detach();

    SimpleObserver obs;
    Client c(obs, io_service, endpoint_iterator);

    QApplication app(argc, argv);
    MainWindow window(c);
    window.show();

    app.exec();
    server_io.stop();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
