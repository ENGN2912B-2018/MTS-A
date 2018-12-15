#include <iostream>           // for std::cout
#include "server.h"
#include "image/image.h"

using std::pair;
using std::make_pair;

// TODO(la): when we get a message write it to some temporary file
namespace server {

const string filename = "/tmp/server_output.txt";
const char *END = "END";
const int END_LENGTH = 3;

// session methods

Session::Session(boost_socket socket) : socket_(std::move(socket)) { }

void Session::start() {
  do_read();
}

void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(buffer(data_, BUFFER_SIZE), [this, self](error_code ec, size_t length) {
    if (!ec) {
      // write given to some output file - commented out because it isn't needed
      //std::ofstream outfile(filename, std::ios::out | std::ios_base::app);
      //outfile << "\n" << data_;
      //outfile.close();
      //bool containsEnd = false;
      //for (int i = 0; i < length; i++) {
      //  for (int j = 0; j < END_LENGTH; j++) {
      //    if (data_[i] != END[j]) { break; }
      //    if (j == END_LENGTH - 1) containsEnd = true;
      //  }
      //}

      do_read();
    }
  });
}

void Session::do_write() {
  auto self(shared_from_this());
  async_write(socket_, buffer("agknowledged!", strlen("agknowledged!")), [this, self](error_code ec, size_t len) {
    if (!ec) {
      do_read();
    } else { std::cout << "error on server side: " << ec.message() << std::endl; }
  });
}


// server methods

Server::Server(io_service& io_service, short port) :
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
  socket_(io_service) { do_accept(); }


void Server::do_accept() {
  acceptor_.async_accept(socket_, [this](error_code ec) {
    if (!ec) {
      std::cout << "server accpected client!" << std::endl;
      std::make_shared<Session>(std::move(socket_))->start();
    }

    do_accept();
  });
}

}  // namespaces server
