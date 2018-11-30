#pragma once

#include <deque>               // for deque
#include <string>              // for string
#include <vector>              // for vector
#include <cstring>             // for strlen
#include <cstddef>             // for size_t
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


// TODO(la): seperate openning socket from creation?
// TODO(la): currently this client has a 'read()' private member function. So,
// what this class does with a message is fixed. Could add a 'Handler/Listener'
// object that will take the data that we read. After reading from the buffer, we
// just pass the value along


namespace client {

/// A class representing the client of our application.
class Client {
  public:
    //= Constructor =============================================================
    /// Constructs Client with an io_service for writing and iterator for finding the endpoint.
    Client(io_service& io_service, resolver_iterator endpoint_iterator);

    //= Public Operations =============================================================
    /// Sends a message to the server.
    void write(const char* msg);

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

    char res_msg_[1024];            // stores responses from server
    message_queue send_queue_;      // queue for sending message
    message_queue receive_queue_;   // queue for received messages
};

}  // namespace client
