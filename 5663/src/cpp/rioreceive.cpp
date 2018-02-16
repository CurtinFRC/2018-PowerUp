#include "rioreceive.h"

void rioreceive::init()
{
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		std::cerr << "Error: Could not create socket" << std::endl;
	} else {
		std::memset((char*)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
			std::cerr << "Error: could not bind socket" << std::endl;
		}
	}
}

unsigned int bytes_to_int(uint8_t *bytes, int &a) {
	unsigned int ret_value = 0;
	ret_value += (unsigned int)(*(bytes+a+0))<<24;//*0x1000000;
	ret_value += (unsigned int)(*(bytes+a+1))<<16;//*0x10000;
	ret_value += (unsigned int)(*(bytes+a+2))<<8;//*0x100;
	ret_value += (unsigned int)(*(bytes+a+3));

	a += 4;

	return ret_value;
}

enum portion {
	NONE,
	CENTRE_XS,
	ANGLES,
	DISTANCES,
	MESSAGE
};

void rioreceive::get(std::vector<int> &centre_xs, std::vector<double> &angles, std::vector<double> &distances, std::string &message)
{
	centre_xs.clear();
	angles.clear();
	distances.clear();
	message = "";
	uint8_t buff[1024];
	std::memset(buff, 0, 1024);
	recvfrom(sockfd, buff, 1024, 0, (struct sockaddr *)&addr, (unsigned int *)1024);
	int data = 0, a = 0;
	portion p = NONE;
	for (int i = 0; i < 1024 && data != 0xE0F; i++) {
		if (data == 0xEC5 || data == 0xDC5) {
			p = CENTRE_XS;
		} else if (data == 0xDE6) {
			p = ANGLES;
		} else if (data == 0xD15) {
			p = DISTANCES;
		} else if (data == 0xAC2) {
			p = MESSAGE;
		} else if (p == CENTRE_XS) {
			centre_xs.push_back(data);
		} else if (p == ANGLES) {
			angles.push_back((double)data/1024-0.436332313);
		} else if (p == DISTANCES) {
			distances.push_back((double)data/1000);
		} else if (p == MESSAGE) {
			message += (char)data;
		}
		data = bytes_to_int(buff, a);
	}
}

void rioreceive::test()
{
	uint8_t buff[1024];
	std::memset(buff, 0, 1024);
	recvfrom(sockfd, buff, 1024, 0, (struct sockaddr *)&addr, (unsigned int *)1024);
	int a = 0;
	for (int i = 0; i < 256; i++) {
		int thing = bytes_to_int(buff, a);
		if (i != thing)
			std::cout << i << " != " << thing << std::endl;
	}
}
