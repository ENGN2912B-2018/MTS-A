#pragma once

#include <memory>
#include <thread>              // for ???
#include <utility>
#include <cstdlib>
#include <iostream>

#include <string>              // for string
#include <cstring>             // for strlen
#include <deque>               // for deque
#include <cstddef>             // for size_t
#include <boost/asio.hpp>      // for boost::asio
#include <boost/bind.hpp>


using std::deque;
using std::string;
using std::size_t;

using namespace boost::asio;
using boost::system::error_code;
using boost::asio::ip::tcp;

typedef boost::asio::ip::tcp::acceptor acceptor;
typedef boost::asio::ip::tcp::socket boost_socket;
typedef boost::asio::ip::tcp::resolver::iterator resolver_iterator;


// TODO(la): depending on our later representation of message values we should
// change these. Or we could this templated Message class and require using that.
typedef char* message;
typedef char* response;
typedef deque<message> message_queue;

class Session : public std::enable_shared_from_this<Session> {
  public:
    Session(boost_socket socket) : socket_(std::move(socket)) { }

    void start() {
      //std::cout << "in session start" << std::endl;
      do_read();
    }

  private:
    void do_read() {
      //std::cout << "in session do_read" << std::endl;
      auto self(shared_from_this());
      socket_.async_read_some(buffer(data_, max_length), [this, self](error_code ec, size_t length) {
        if (!ec) {
          std::cout << "server recieved message: " << data_<< std::endl;
          do_write(length);
        }
      });
    }

    void do_write(size_t length) {
      auto self(shared_from_this());
      async_write(socket_, buffer("agknowledged!", strlen("agknowledged!")), [this, self](error_code ec, size_t len) {
        if (!ec) {
          do_read();
        } else { std::cerr << "error on server side: " << ec.message() << std::endl; }
      });
    }

    boost_socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};



class Server {
  public:
    Server(io_service& io_service, short port) :
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service) {
        do_accept();
      }

  private:
    void do_accept() {
      acceptor_.async_accept(socket_, [this](error_code ec) {
        if (!ec) {
          std::cout << "server accpected client!" << std::endl;
          std::make_shared<Session>(std::move(socket_))->start();
        }

        do_accept();
      });
    }

    // private fields
    acceptor acceptor_;
    boost_socket socket_;
};
