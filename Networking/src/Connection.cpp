//
// Created by Kekuch13 on 14.03.2023.
//

#include <Networking/Connection.h>

#include <memory>

Connection::Connection(net::io_context &ioc) : socket(ioc) {};

std::shared_ptr<Connection> Connection::create(net::io_context &ioc) {
    return std::make_shared<Connection>(ioc);
}

void Connection::sendResponse(http::message_generator &resp) {
    beast::error_code ec;
    beast::write(socket, std::move(resp), ec);

    if (ec)
        std::cerr << "Error on writing:" << ec.message();
}

http::message_generator Connection::handle_request() {
    std::cout << "----------\n" << req.method_string() << std::endl << req.target() << std::endl;

    // Returns a bad request response
    auto const bad_request = [this](beast::string_view why) {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error = [this](beast::string_view what) {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    switch(req.method()) {
        case http::verb::post:
            if(req.target() == "/account") {
//                auto res = addAccount();
//                return res;
            }
            if(req.target() == "/expenses") {
//                auto res = addExpanse();
//                return res;
            }
            if(req.target() == "/income") {
//                auto res = addIncome();
//                return res;
            }
            if(req.target().starts_with("/categories")) {
//                auto res = addCategory();
//                return res;
            }
            break;
        case http::verb::put:
            if(req.target() == "/account") {
//                auto res = renameAccount();
//                return res;
            }
            if(req.target() == "/expenses") {
//                auto res = modifyExpanse();
//                return res;
            }
            if(req.target() == "/income") {
//                auto res = modifyIncome();
//                return res;
            }
            if(req.target().starts_with("/categories")) {
//                auto res = renameCategory();
//                return res;
            }
            break;
        case http::verb::get:
            if(req.target().starts_with("/account")) {
//                auto res = getAccount();
//                return res;
            }
            if(req.target().starts_with("/expenses")) {
//                auto res = getExpanse();
//                return res;
            }
            if(req.target().starts_with("/income")) {
//                auto res = getIncome();
//                return res;
            }
            if(req.target().starts_with("/categories")) {
//                auto res = getCategory();
//                return res;
            }
            break;
        case http::verb::delete_:
            if(req.target().starts_with("/account")) {
//                auto res = deleteAccount();
//                return res;
            }
            if(req.target().starts_with("/expenses")) {
//                auto res = deleteExpanse();
//                return res;
            }
            if(req.target().starts_with("/income")) {
//                auto res = deleteIncome();
//                return res;
            }
            if(req.target().starts_with("/categories")) {
//                auto res = deleteCategory();
//                return res;
            }
            break;
        default:
            return bad_request("Unknown HTTP-method");
            break;
    }
    //временная заглушка
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = req.target();
    res.prepare_payload();
    return res;
};

void Connection::Start() {
    beast::error_code ec;
    while (true) {
        http::read(socket, buffer, req, ec);
        if (ec == http::error::end_of_stream) {
            break;
        }
        if (ec) {
            std::cerr << "Fail on reading: " << ec.message() << std::endl;
            break;
        }
        http::message_generator msg = handle_request();
        sendResponse(msg);
    }
    socket.shutdown(tcp::socket::shutdown_send, ec);
    std::cout << "Connection closed\n";
};

