#include <Server/Server.h>

Server::Server(const net::ip::address &address, unsigned short port)
    : acceptor{ioc, {address, port}}, socket{ioc} {}

void Server::AcceptClient() {
    acceptor.async_accept(socket, [this](const beast::error_code &error) {
        auto conn = Connection::create(std::move(socket));
        std::cout << "Client accepted!\n";

        if (!error) conn->start();

        AcceptClient();
    });
}

int Server::run() {
    try {
        AcceptClient();
        ioc.run();
    } catch (const std::exception &e) {
        std::cerr << e.what();
        throw;
    }
    return EXIT_SUCCESS;
}
