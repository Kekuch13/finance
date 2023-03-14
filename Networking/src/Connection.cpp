//
// Created by User on 14.03.2023.
//

#include <Networking/Connection.h>

Connection::Connection(net::io_context& ioc) : socket(ioc) {};

std::shared_ptr<Connection> Connection::create(net::io_context& ioc) {
    return std::shared_ptr<Connection>(new Connection(ioc));
};


