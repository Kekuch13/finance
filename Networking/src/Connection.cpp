//
// Created by Kekuch13 on 14.03.2023.
//

#include <Networking/Connection.h>

Connection::Connection(net::io_context &ioc) : socket(ioc) {};

std::shared_ptr<Connection> Connection::create(net::io_context &ioc) {
    return std::shared_ptr<Connection>(new Connection(ioc));
};

http::message_generator resp(http::request<http::string_body>& req) {
    //временная заглушка
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occurred: ";
    res.prepare_payload();
    return res;
};

void Connection::process_request() {
    std::cout << "----------\n" << req.target() << std::endl << req.method_string() << std::endl;

    beast::error_code ec;
    http::message_generator msg = resp(req);
    beast::write(socket, std::move(msg), ec);

    if(ec)
        std::cerr << "Error on writing:" << ec.message();

//    switch(req.method()) {
//        case http::verb::get:
//            break;
//
//    }
};

void Connection::handle_request() {
    beast::error_code ec;
    while (true) {
        http::read(socket, buffer, req, ec);
        if (ec == http::error::end_of_stream) {
            break;
        }
        if(ec) {
            std::cerr << "Fail on reading: " << ec.message() << std::endl;
        }
        process_request();

        // выход из цикла
    }
    socket.shutdown(tcp::socket::shutdown_send, ec);
}
void Connection::Start() {
    handle_request();
    std::cout << "Connection closed\n";
};

