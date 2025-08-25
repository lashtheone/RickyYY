#include "../include/common.hpp"
#include <iostream>


int main(int argc, char** argv) {
	unsigned short port = 9000; // 默认端口
	if (argc >= 2) port = static_cast<unsigned short>(std::stoi(argv[1]));


	Logger log; log.line("server", "boot on port " + std::to_string(port));


	int lfd = tcp_listen(port);
	if (lfd < 0) { std::cerr << "listen failed\n"; return 1; }
	std::cout << "[server] listening on 0.0.0.0:" << port << " ...\n";


	while (true) {
		int cfd = tcp_accept(lfd);
		if (cfd < 0) { std::cerr << "accept failed\n"; continue; }
		std::cout << "[server] client connected\n";
		log.line("server", "client connected");

		send_line(cfd, "Commands:");
		send_line(cfd, "  /logs - List all log files");
		send_line(cfd, "  /read <filename> - Read specified log file");
		send_line(cfd, "  /exit - Exit client");

		std::string line;
		while (recv_line(cfd, line)) {
			if (line == "/logs") {
				list_log_files(cfd, log);
			}
			else if (line.rfind("/read ", 0) == 0) {
				std::string filename = line.substr(6);
				read_log_file(cfd, filename, log);
				send_line(cfd, "> ");
			}
			else if (line == "/exit") {
				send_line(cfd, "Goodbye!");
				break;
			}
			else {
				log.line("client", line);
				std::cout << "[server] received: " << line << std::endl;

				if (!send_line(cfd, "server: " + line)) break;
				log.line("server", std::string("server: ") + line);
				std::cout << "[server] sent: server: " << line << std::endl;
			}
		}
		log.line("server", "client disconnected");
		std::cout << "[server] client disconnected\n";
		close_fd(cfd);
	}
	close_fd(lfd);
	return 0;
}