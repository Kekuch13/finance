//
// Created by Kekuch13 on 14.03.2023.
//
#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
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

class Connection : public std::enable_shared_from_this<Connection> {
private:
    tcp::socket socket;
    http::request<http::string_body> req;
    beast::flat_buffer buffer;
public:
    explicit Connection(net::io_context &ioc);

    static std::shared_ptr<Connection> create(net::io_context &ioc);

    tcp::socket &GetSocket() {
        return socket;
    }

    void Start();
private:
    void handle_request();
    void process_request();
};
