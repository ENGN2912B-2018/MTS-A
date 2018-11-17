#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;


class Server{

public:
    Server(boost::asio::io_service& io_service, short port);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};
