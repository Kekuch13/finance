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
    static std::shared_ptr<Connection> create(tcp::socket &&socket);
    void start();

private:
    explicit Connection(tcp::socket &&socket);

    void asyncRead();
    void onRead(const beast::error_code &error, std::size_t bytes_transferred);

    void asyncWrite(http::message_generator &&msg);
    void onWrite(const beast::error_code &error, std::size_t, bool keep_alive);

    void handleRequest();

    void badRequest(beast::string_view why); // Returns a bad request response
    void successResponse(http::status status); // Returns a successful responses
    void jsonResponse(beast::string_view data); // Return success response with json body

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
    bool recordExists(int id, const std::string& tableName);
    boost::property_tree::ptree toJson(pqxx::result &res);
};
