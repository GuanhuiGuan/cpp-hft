#include <iostream>

#include <fcntl.h>
#include <unistd.h>

void testRead() {
    int fd = open("test-read.txt", O_RDWR);
    if (fd < 0) {
        std::cerr << "open err: " << strerror(errno) << '\n';
        return;
    }

    char buffer[8];
    size_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        std::cout.write(buffer, bytesRead);
    }
    if (bytesRead < 0) {
        std::cerr << "read err: " << strerror(errno) << '\n';
        close(fd);
        return;
    }
    std::cout << std::endl;

    close(fd);
}

void testWrite() {
    int fd = open("test-write.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        std::cerr << "open err: " << strerror(errno) << '\n';
        return;
    }

    if (ftruncate(fd, 1024) < 0) {
        std::cerr << "ftruncate err: " << strerror(errno) << '\n';
        close(fd);
        return;
    }

    const char* content = "May the force be with you.\n";
    size_t bytesWritten = write(fd, content, strlen(content));
    std::cout << bytesWritten << "B written\n";
    
    close(fd);
}

int main() {
    testRead();
    testWrite();
    
    return 0;
}