#include <iostream>           // for std::cout
#include <cstring>
#include "client.h"

namespace client {

Client::Client(io_service &io_service, resolver_iterator endpoint_iterator)
  : io_service_(io_service), socket_(io_service) {
  do_connect(endpoint_iterator);
}

Client::Client(Observer observer, io_service &io_service, resolver_iterator endpoint_iterator)
  : observer_(observer), io_service_(io_service), socket_(io_service) {
  do_connect(endpoint_iterator);
}

Client::~Client() { close(); }

void Client::write(const char* msg) {
  io_service_.post([this, msg]() {
    bool write_in_progress = !send_queue_.empty();
    send_queue_.push_back(msg);
    if (!write_in_progress) { do_write(); }
  });
}

void Client::write(vector<bool> &msg) {
  char data[msg.size() + 4];
  for (int i = 0; i < msg.size(); i++)
    data[i] = msg.at(i) ? '1' : '0';
  data[msg.size()] = 'E';
  data[msg.size() + 1] = 'N';
  data[msg.size() + 2] = 'D';
  data[msg.size() + 3] = '\0';
  write(data);
}

void Client::close() {
  io_service_.post([this]() { socket_.close(); });
}


// private members below

void Client::do_write() {
  const char* msg = send_queue_.front();
  int length = strlen(msg);
  async_write(socket_, buffer(msg, BUFFER_SIZE), [this](error_code ec, size_t len) {
    if (!ec) {
      send_queue_.pop_front();
      if (!send_queue_.empty()) do_write();
    }
  });
}

void Client::do_read() {
  // here we have to use async_read_some, because async_read completition conditions aren't flexible
  socket_.async_read_some(buffer(res_msg_, BUFFER_SIZE), [this](error_code ec, size_t len) {
    if (!ec) {
      observer_.observe(res_msg_);
      do_read();
    } else { std::cout << "client read error: " + ec.message() << std::endl; }
  });
}

void Client::do_connect(resolver_iterator endpoint_iterator) {
  async_connect(socket_, endpoint_iterator, [this](error_code ec, resolver_iterator it) {
    if (!ec) { do_read(); }
  });
}

}  // namespace client
