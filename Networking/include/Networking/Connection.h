//
// Created by Kekuch13 on 14.03.2023.
//

#pragma once

#include <Networking/DatabaseManager.h>

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
    DatabaseManager dbManager;
public:
    explicit Connection(net::io_context &ioc);

    static std::shared_ptr<Connection> create(net::io_context &ioc);

    tcp::socket &GetSocket();

    void Start();
private:
    void bad_request(beast::string_view why); // Returns a bad request response
    void server_error(beast::string_view what); // Returns a server error response
    void success(http::status status); // Returns a successful response

    void addAccount();
    void addExpense();
    void addIncome();
    void addCategory();

    void renameAccount();
    void modifyExpense();
    void modifyIncome();
    void renameCategory();

    void getAccount();
    void getExpense();
    void getIncome();
    void getCategory();

    void deleteAccount();
    void deleteExpense();
    void deleteIncome();
    void deleteCategory();

    void handle_request();

    void sendResponse(http::message_generator &&res);
};
