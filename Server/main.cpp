#include <iostream>

#include <Networking/Server.h>

int main() {
    std::cout << "Hello, It's Server!\n";
    try {
        Server server(net::ip::make_address("127.0.0.1"), 8080);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}