#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

class WebServer {
    const char* ip_;
    unsigned short int port_;

    bool healthy_ {true};

    int srvSock {-1};
    sockaddr_in srvAddr;
    int srvAddrLen;
    char recvBuffer[1024];
public:
    WebServer(const char* ip, unsigned short int port, int sockFamily = AF_INET, int sockType = SOCK_STREAM, int protocol = 0) : ip_{ip}, port_{port} {
        // create socket
        if ((srvSock = socket(sockFamily, sockType, protocol)) < 0) {
            healthy_ = false;
            std::cerr << "socket err: " << errno << '\n';
            return;
        }
        // prepare socket address
        srvAddr.sin_family = sockFamily;
        srvAddr.sin_port = htons(port);
        if (inet_aton(ip, &srvAddr.sin_addr) < 0) {
            healthy_ = false;
            std::cerr << "address to network order bytes err: " << errno << '\n';
            return;
        }
        srvAddrLen = sizeof(srvAddr);
    }
    ~WebServer() noexcept {
        shutdown();
    }

    bool bindAndListen() {
        // bind socket to the address
        if (bind(srvSock, (sockaddr*)&srvAddr, srvAddrLen) < 0) {
            std::cerr << "bind err: " << errno << '\n';
            return false;
        }
        // listen to the address
        if (listen(srvSock, 3) < 0) {
            std::cerr << "listen err: " << errno << '\n';
            return false;
        }
        std::cout << "listening on " << ip_ << ":" << port_ << '\n';
        return true;
    }

    bool connectAndSend(const char* msg) {
        // build connection
        if (connect(srvSock, (sockaddr*)&srvAddr, srvAddrLen) < 0) {
            std::cerr << "connect err: " << errno << '\n';
            return false;
        }
        // send
        int byteSent = send(srvSock, msg, strlen(msg), 0);
        std::cout << byteSent << " bytes sent\n";
        if (byteSent < 0) {
            std::cerr << "send err: " << errno << '\n';
            return false;
        }
        return true;
    }

    // receive only 1 msg
    void recvMsg() {
        int childSock;
        if ((childSock = accept(srvSock, (sockaddr*)&srvAddr, (socklen_t*)&srvAddrLen)) < 0) {
            std::cerr << "accept err: " << errno << '\n';
            return;
        }
        memset(recvBuffer, 0, strlen(recvBuffer));
        if (recv(childSock, recvBuffer, sizeof(recvBuffer), 0) < 0) {
            std::cerr << "recv err: " << errno << '\n';
            close(childSock);
            return;
        }
        std::cout << "receved msg: " << recvBuffer << '\n';
        close(childSock);
    }

    void loopSelect() {
        fcntl(srvSock, F_SETFL, fcntl(srvSock, F_GETFL, 0) | O_NONBLOCK);

        fd_set readFds;
        // fd_set writeFds;
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        while (true) {
            // register FDs to monitor
            FD_ZERO(&readFds);
            FD_SET(srvSock, &readFds);
            // first arg "nfds: num of fd" notes that we should monitor [0, (nfds-1)]
            int ret = select(srvSock+1, &readFds, nullptr, nullptr, &timeout);
            if (ret == -1) {
                std::cerr << "select err: " << errno << '\n';
            } else if (ret) {
                recvMsg();
            } else {
                std::cout << "no connection\n";
            }
            sleep(1);
        }
    }

    bool healthy() const {
        return healthy_;
    }

    void shutdown() {
        if (srvSock >= 0) close(srvSock);
        std::cout << "server shutdown\n";
    }
};