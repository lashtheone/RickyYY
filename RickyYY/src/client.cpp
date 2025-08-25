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

	// ���Ƚ��ղ���ʾ�������Ļ�ӭ��Ϣ��������ʾ
	while (recv_line(fd, reply)) {
		std::cout << reply << "\n";
		log.line("server", reply);
		// �����⵽��ʾ�����ض�������ǣ���������ѭ��
		if (reply == "> " || reply.find("Enter your command") != std::string::npos) {
			break;
		}
	}

	while (true) {
		std::cout << "> ";  // �ͻ����Լ���ʾ��ʾ��
		if (!std::getline(std::cin, line)) break;
		if (line == "/exit") break;

		if (!send_line(fd, line)) { std::cerr << "send failed\n"; break; }
		log.line("client", line);

		// ���շ����������лظ���������ʾ����
		while (recv_line(fd, reply)) {
			// ����յ�������ʾ������ֹͣ���գ�������ʾ��
			if (reply == "> ") {
				break;
			}
			std::cout << reply << "\n";  // ��ʾʵ������
			log.line("server", reply);
		}
	}
	close_fd(fd);
	return 0;
}