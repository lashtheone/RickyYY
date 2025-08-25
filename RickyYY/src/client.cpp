#include "../include/common.hpp"
#include <iostream>


int main(int argc, char** argv) {
	if (argc < 3) {
		std::cerr << "usage: simple_client <host> <port>\n";
		return 1;
	}
	std::string host = argv[1];
	unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));


	Logger log; log.line("client", "connect to " + host + ":" + std::to_string(port));


	int fd = tcp_connect(host, port);
	if (fd < 0) { std::cerr << "connect failed\n"; return 1; }


	std::string line, reply;

	// 首先接收并显示服务器的欢迎信息和命令提示
	while (recv_line(fd, reply)) {
		std::cout << reply << "\n";
		log.line("server", reply);
		// 如果检测到提示符或特定结束标记，可以跳出循环
		if (reply == "> " || reply.find("Enter your command") != std::string::npos) {
			break;
		}
	}

	while (true) {
		std::cout << "> ";  // 客户端自己显示提示符
		if (!std::getline(std::cin, line)) break;
		if (line == "/exit") break;

		if (!send_line(fd, line)) { std::cerr << "send failed\n"; break; }
		log.line("client", line);

		// 接收服务器的所有回复（除了提示符）
		while (recv_line(fd, reply)) {
			// 如果收到的是提示符，就停止接收，但不显示它
			if (reply == "> ") {
				break;
			}
			std::cout << reply << "\n";  // 显示实际内容
			log.line("server", reply);
		}
	}
	close_fd(fd);
	return 0;
}