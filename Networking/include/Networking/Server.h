//
// Created by Kekuch13 on 14.03.2023.
//

#pragma once

#include <Networking/Connection.h>

#include <thread>

class Server {
private:
    net::io_context ioc{1};
    tcp::acceptor acceptor;
public:
    Server(const net::ip::address &address, unsigned short port);

    int run();

    void AcceptClient();

    static void session(std::shared_ptr<Connection> conn) {
        conn->Start();
    }
};
