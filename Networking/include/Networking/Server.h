//
// Created by Kekuch13 on 14.03.2023.
//

#pragma once

#include <Networking/Connection.h>

#include <iostream>
#include <vector>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <memory>
#include <string>
#include <boost/asio.hpp>

//#include <pqxx/pqxx>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Server {
private:
    net::io_context ioc{1};
    tcp::acceptor acceptor;
    std::vector<std::shared_ptr<Connection>> connections;
public:
    Server(const net::ip::address& address, unsigned short port);

    void run();
};