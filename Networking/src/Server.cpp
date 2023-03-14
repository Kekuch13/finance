//
// Created by User on 14.03.2023.
//

#include <Networking/Server.h>

Server::Server(const net::ip::address& address, unsigned short port)
    : acceptor{ioc, {address, port}}
    {};

void Server::run() {
    while(true) {
        auto conn = Connection::create(ioc);
        connections.push_back(conn);

        // Block until we get a Connection
        acceptor.accept(conn->GetSocket());
        std::cout << "Client accepted!\n";
    }
};

