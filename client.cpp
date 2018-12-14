#include <string>
#include <thread>
#include <cassert>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <QApplication>

#include "gui/mainwindow.h"
#include "image/image.h"
#include "client/client.h"

using namespace gui;
using namespace client;

using std::vector;
using boost::asio::ip::tcp;


/// The main client executable.
int main(int argc, char* argv[]) {
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
    t.detach();

    SimpleObserver obs;
    Client c(obs, io_service, endpoint_iterator);

    QApplication app(argc, argv);
    MainWindow window(c);
    window.show();

    return app.exec();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return -1;
}
