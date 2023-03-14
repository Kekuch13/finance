//
// Created by Kekuch13 on 14.03.2023.
//
#pragma once

#include <iostream>

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

class Connection : public std::enable_shared_from_this<Connection> {
private:
    tcp::socket socket;
    http::request<http::string_body> req;
public:
    explicit Connection(net::io_context& ioc);

    static std::shared_ptr<Connection> create(net::io_context& ioc);

    tcp::socket& GetSocket() {
        return socket;
    }
};
