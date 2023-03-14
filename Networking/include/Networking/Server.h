//
// Created by Kekuch13 on 14.03.2023.
//
#pragma once

#include <Networking/Connection.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/asio.hpp>

//#include <pqxx/pqxx>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Server {
private:
    net::io_context ioc{1};
    tcp::acceptor acceptor;
public:
    Server(const net::ip::address& address, unsigned short port);

    int run();

    void AcceptClient();

    static void session(std::shared_ptr<Connection> conn) {
        conn->Start();
    }
};