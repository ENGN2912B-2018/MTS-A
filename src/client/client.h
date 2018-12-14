#pragma once

#include <deque>               // for deque
#include <string>              // for string
#include <vector>              // for vector
#include <cstring>             // for strlen
#include <cstddef>             // for size_t
#include <iostream>            // for cout
#include <boost/asio.hpp>      // for boost::asio

using std::deque;
using std::string;
using std::vector;
using std::size_t;

using namespace boost::asio;
using boost::system::error_code;

typedef deque<const char *> message_queue;
typedef boost::asio::ip::tcp::socket boost_socket;
typedef boost::asio::ip::tcp::resolver::iterator resolver_iterator;


namespace client {

const int BUFFER_SIZE = 1000000;

/// An interface used to specify an Object that would view what messages the client receives
class Observer {
  public:
    /// Receives a message from the
    virtual void observe(const char *message) { }
};

/// An Observer object that alerts that when a message is recieved.
class SimpleObserver : public Observer {
  public:
    void observe(const char *message) override {
      std::cout << "received message: " << message << std::endl;
    }
};


/// A class representing the client of our application.
/// The Client object can be used to send and recieve messages from a specificied
/// endpoints. Received messages can be observed by providing a custom Observer
/// object to the Client.
class Client {
  public:
    //= Constructor =============================================================
    /// Constructs a Client with an io_service for writing and iterator for finding the endpoint.
    Client(io_service &io_service, resolver_iterator endpoint_iterator);

    /// Constructs a Client with an Observer object that will observe all messgaes
    /// that the client receives.
    Client(Observer observer, io_service &io_service, resolver_iterator endpoint_iterator);

    /// Destructor.
    ~Client();

    //= Public Operations =============================================================
    /// Sends a message to the server.
    void write(const char* msg);

    /// Sends a message as a vector of boolean to the server.
    void write(vector<bool> &data);

    /// Closes the socket.
    void close();

  private:
    //= Private Operations =============================================================
    /// Performs the actual 'send message' operation.
    void do_write();

    /// Reads any messages the socket recieves.
    void do_read();

    /// Connects the socket to the server.
    void do_connect(resolver_iterator endpoint_iterator);

    //= Member Variables =============================================================
    boost_socket socket_;           // socket for sending & receiving messages
    io_service &io_service_;        // io_service for socket

    char res_msg_[BUFFER_SIZE];     // stores responses from server
    message_queue send_queue_;      // queue for sending message
    Observer observer_;              // an observer object to handle received messages
};

}  // namespace client
