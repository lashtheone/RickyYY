#include "../include/common.hpp"
#include <iostream>


int main(int argc, char** argv) {
	unsigned short port = 9000; // Ĭ�϶˿�
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


		std::string line;
		while (recv_line(cfd, line)) {
			// ��¼��˭����/���ݡ�
			log.line("client", line);
			// ���������򵥵Ļ���Э�飺�յ�ʲô�ͻ�ʲô
			std::cout << "[server] received: " << line << std::endl;   // ?? ������ӡ
			
			if (!send_line(cfd, "server: " + line)) break;
			log.line("server", std::string("server: ") + line);
			std::cout << "[server] sent: server: " << line << std::endl; // ?? ������ӡ
		}
		log.line("server", "client disconnected");
		std::cout << "[server] client disconnected\n";
		close_fd(cfd);
	}
	close_fd(lfd);
	return 0;
}