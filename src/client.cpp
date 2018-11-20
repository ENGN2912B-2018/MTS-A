#include <iostream>           // for std::cout
#include "client.h"


Client::Client(io_service &io_service, resolver_iterator endpoint_iterator)
      : io_service_(io_service), socket_(io_service) {
        do_connect(endpoint_iterator);
        std::cout << "created client!" << std::endl;
      }


void Client::write(const message &msg) {
  std::cout << "about to write!" << std::endl;
  io_service_.post([this, msg]() {
    std::cout << "in callback" << std::endl;
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
  std::cout << "doing write" << std::endl;
  message str = queue_.front();
  async_write(socket_, buffer(str, strlen(str)), [this](error_code ec, size_t len) {
    std::cout << "in write callback" << std::endl;
    if (!ec) {
      queue_.pop_front();
      if (!queue_.empty()) do_write();
    } else {
      socket_.close();
    }
  });
}

void Client::do_read() {
  async_read(socket_, buffer(res_msg_, 128),[this](error_code ec, size_t len) {
    if (!ec) {
      std::cout << "in do_read callback!" << std::endl;
      std::cout.write(res_msg_, strlen(res_msg_));
      std::cout << "\n";
      do_read();
    } else {
      socket_.close();
    }
  });
}

void Client::do_connect(resolver_iterator endpoint_iterator) {
  async_connect(socket_, endpoint_iterator, [this](error_code ec, resolver_iterator it) {
    if (!ec) do_read();
  });
}
