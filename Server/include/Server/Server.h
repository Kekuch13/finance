#pragma once

#include <Server/Connection.h>

#include <thread>

class Server {
private:
    net::io_context ioc{1};
    tcp::acceptor acceptor;
    tcp::socket socket;

public:
    Server(const net::ip::address &address, unsigned short port);

    int run();
    void AcceptClient();
};
