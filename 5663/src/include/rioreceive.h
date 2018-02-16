#pragma once

#ifdef _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
#endif
#include <vector>
#include <cstring>
#include <string>
#include <cstdint>
#include <iostream>

const int PORT = 0xDEAD;

class rioreceive
{
	public:
		void init();
		void get(std::vector<int> &centre_xs, std::vector<double> &angles, std::vector<double> &distances, std::string& message);
		void test();

	private:
		int sockfd;
		struct sockaddr_in addr;
};
