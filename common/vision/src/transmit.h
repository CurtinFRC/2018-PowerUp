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
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstring>
#include "config.h"

const char IP_5663[] = "10.56.63.2", IP_5333[] = "10.53.33.2";
const int PORT = 0xDEAD;

class transmit
{
	public:
		void transminit(const char* ip);
		void send(std::vector<int> centre_xs, std::vector<int> distances, const char* message);
		void test();

	private:
		int sockfd;
		struct sockaddr_in addr;
};
