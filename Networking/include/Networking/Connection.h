//
// Created by Kekuch13 on 14.03.2023.
//
#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <stdexcept>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    http::message_generator addAccount();
    http::message_generator addExpanse();
    http::message_generator addIncome();
    http::message_generator addCategory();

    http::message_generator renameAccount();
    http::message_generator modifyExpanse();
    http::message_generator modifyIncome();
    http::message_generator renameCategory();

    http::message_generator getAccount();
    http::message_generator getExpanse();
    http::message_generator getIncome();
    http::message_generator getCategory();

    http::message_generator deleteAccount();
    http::message_generator deleteExpanse();
    http::message_generator deleteIncome();
    http::message_generator deleteCategory();

    http::message_generator handle_request();
    void sendResponse(http::message_generator& resp);
};
