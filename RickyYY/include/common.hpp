#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

inline std::string now_ts() {
    using namespace std::chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    std::tm lt{};
#ifdef _WIN32
    localtime_s(&lt, &t);
#else
    localtime_r(&t, &lt);
#endif
    std::ostringstream oss; oss << std::put_time(&lt, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline std::string today_date() {
    using namespace std::chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    std::tm lt{};
#ifdef _WIN32
    localtime_s(&lt, &t);
#else
    localtime_r(&t, &lt);
#endif
    std::ostringstream oss; oss << std::put_time(&lt, "%Y-%m-%d");
    return oss.str();
}

struct Logger {
    std::ofstream ofs;
    explicit Logger(const std::string& base_dir = "logs") {
        std::filesystem::create_directories(base_dir);
        auto path = base_dir + "/" + today_date() + ".log";
        ofs.open(path, std::ios::app);
    }
    void line(const std::string& who, const std::string& text) {
        ofs << now_ts() << " [" << who << "] " << text << "\n";
        ofs.flush();
    }
};

int tcp_listen(unsigned short port);
int tcp_accept(int listen_fd);
int tcp_connect(const std::string& host, unsigned short port);
bool send_line(int fd, const std::string& s);
bool recv_line(int fd, std::string& out);
void close_fd(int fd);
