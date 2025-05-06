#include "web-server.h"

int main() {
    WebServer ws("127.0.0.1", 8080);
    if (!ws.healthy()) return -1;
    ws.bindAndListen();
    // ws.recvMsg();
    ws.loopSelect();

    return 0;
}