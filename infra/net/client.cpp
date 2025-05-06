#include "web-server.h"

int main() {
    WebServer ws("127.0.0.1", 8080);
    std::cout << "health: " << ws.healthy() << '\n';
    if (!ws.healthy()) return -1;
    ws.connectAndSend("Hello there!");

    return 0;
}