#include "../include/common.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <cstring>

int tcp_listen(unsigned short port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    int yes = 1; setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(port);
    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { ::close(fd); return -1; }
    if (listen(fd, 16) < 0) { ::close(fd); return -1; }
    return fd;
}

int tcp_accept(int listen_fd) {
    sockaddr_in cli{}; socklen_t len = sizeof(cli);
    int cfd = accept(listen_fd, (sockaddr*)&cli, &len);
    return cfd; // 失败返回 -1
}

int tcp_connect(const std::string& host, unsigned short port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0); if (fd < 0) return -1;

    addrinfo hints{}; hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr; 
    if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) { ::close(fd); return -1; }
    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_port = htons(port);
    addr.sin_addr = ((sockaddr_in*)res->ai_addr)->sin_addr;
    freeaddrinfo(res);

    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { ::close(fd); return -1; }
    return fd;
}

bool send_line(int fd, const std::string& s) {
    std::string data = s; data.push_back('\n');
    const char* p = data.c_str(); size_t n = data.size();
    while (n > 0) {
        ssize_t w = ::send(fd, p, n, 0);
        if (w <= 0) return false; n -= (size_t)w; p += w;
    }
    return true;
}

bool recv_line(int fd, std::string& out) {
    out.clear(); char ch;
    while (true) {
        ssize_t r = ::recv(fd, &ch, 1, 0);
        if (r <= 0) return false;
        if (ch == '\n') break; out.push_back(ch);
        if (out.size() > (1u<<20)) return false; // 单行<=1MB
    }
    return true;
}

void close_fd(int fd) { if (fd >= 0) ::close(fd); }
