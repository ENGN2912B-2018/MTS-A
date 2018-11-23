#pragma once

#include <deque>               // for deque
#include <memory>              // for enable_shared_from_this
#include <string>              // for string
#include <cstring>             // for strlen
#include <cstddef>             // for size_t
#include <boost/asio.hpp>      // for boost::asio


using std::deque;
using std::string;
using std::size_t;

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::system::error_code;

typedef boost::asio::ip::tcp::acceptor acceptor;
typedef boost::asio::ip::tcp::socket boost_socket;
typedef boost::asio::ip::tcp::resolver::iterator resolver_iterator;


// TODO(la): depending on our later representation of message values we should
// change these. Or we could this templated Message class and require using that.
typedef char* message;
typedef char* response;
typedef deque<message> message_queue;

/// A class representing a clients Session.
/// Each session is responsible for communicating with a singular client.
class Session : public std::enable_shared_from_this<Session> {
  public:
    //= Constructor ============================================================
    /// Constructs a Session with a socket for communication.
    Session(boost_socket socket);

    //= Public Operations ======================================================
    /// Starts the session.
    void start();
  private:
    //= Private Operations =====================================================
    /// Reads from the socket.
    void do_read();

    /// Writes at most length bytes of the current the buffer to the socket.
    void do_write(size_t length);

    //= Member Variables =======================================================
    boost_socket socket_;          // the socket we are using for communication
    enum { max_length = 1024 };    // max size used for messages
    char data_[max_length];        // data buffer used for sending messages
};



/// A class representing a Server object.
/// The server is responsible for accepting incoming connections an then creating
/// a session object to handle the client/server interactions.
class Server {
  public:
    //= Constructor ============================================================
    /// Constructs a Server with an io_service for writing and its port.
    Server(io_service& io_service, short port);

  private:
    //= Private Operations =====================================================
    /// Accepts incoming connections.
    void do_accept();

    //= Member Variables =======================================================
    acceptor acceptor_;     // an object that accepts incoming connections
    boost_socket socket_;   // the socket we are using for communication
};
