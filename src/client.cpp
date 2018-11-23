#include <iostream>           // for std::cout
#include "client.h"

Client::Client(io_service &io_service, resolver_iterator endpoint_iterator)
      : io_service_(io_service), socket_(io_service) {
        do_connect(endpoint_iterator);
      }

void Client::write(const message &msg) {
  io_service_.post([this, msg]() {
    bool write_in_progress = !queue_.empty();
    queue_.push_back(msg);
    if (!write_in_progress) { do_write(); }
  });
}

void Client::close() {
  io_service_.post([this]() { socket_.close(); });
}


// private members below


void Client::do_write() {
  message str = queue_.front();
  async_write(socket_, buffer(str, strlen(str)), [this](error_code ec, size_t len) {
    if (!ec) {
      queue_.pop_front();
      if (!queue_.empty()) do_write();
    }
  });
}



void Client::do_read() {
  message str = queue_.front();
  // here we have to use async_read_some, because async_read completition
  // conditions aren't flexible
  socket_.async_read_some(buffer(res_msg_, 128), [this](error_code ec, size_t len) {
    if (!ec) {
      do_read();
      std::cout << "client received message: " << res_msg_ << std::endl;
    } else { std::cout << "client read error: " + ec.message() << std::endl; }
  });
}

void Client::do_connect(resolver_iterator endpoint_iterator) {
  async_connect(socket_, endpoint_iterator, [this](error_code ec, resolver_iterator it) {
    if (!ec) { do_read(); }
  });
}
