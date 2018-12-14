#include <iostream>           // for std::cout
#include "server.h"

namespace server {

// session methods

Session::Session(boost_socket socket) : socket_(std::move(socket)) { }

void Session::start() {
  do_read();
}

void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(buffer(data_, BUFFER_SIZE), [this, self](error_code ec, size_t length) {
    if (!ec) {
      std::cout << "server recieved a message!" << std::endl;
      //std::cout << data_ << std::endl;
      //receive_queue_.push_back(data_);
      do_write();
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
