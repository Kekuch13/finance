//
// Created by Kekuch13 on 14.03.2023.
//

#include <Server/Server.h>

Server::Server(const net::ip::address &address, unsigned short port)
    : acceptor{ioc, {address, port}} {};

void Server::AcceptClient() {
    while (true) {
        auto conn = Connection::create(ioc);

        acceptor.accept(conn->GetSocket());
        std::cout << "Client accepted!\n";
        std::thread(std::bind(&session, conn)).detach();
    }
};

int Server::run() {
    try {
        AcceptClient();
    } catch (const std::exception &e) {
        std::cerr << e.what();
        throw;
    }
    return EXIT_SUCCESS;
};
