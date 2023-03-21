//
// Created by Kekuch13 on 14.03.2023.
//

#pragma once

#include <Server/DatabaseManager.h>

#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

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
    void success_response(http::status status); // Returns a successful responses
    void json_response(beast::string_view &&data); // Return success response with json body

    void addAccount();
    void addExpense();
    void addIncome();
    void addCategory();

    void modifyAccount();
    void modifyExpense();
    void modifyIncome();
    void modifyCategory();

    void getAccount();
    void getExpense();
    void getIncome();
    void getByCategory();

    void deleteAccount();
    void deleteExpense();
    void deleteIncome();
    void deleteCategory();

    std::unordered_map<std::string, std::string> parseQuery();
    bool recordExists(int id, std::string &&table);
    boost::property_tree::ptree toJson(pqxx::result &res);

    void handle_request();
    void sendResponse(http::message_generator &&res);
};
