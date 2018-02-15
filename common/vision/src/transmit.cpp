#include "transmit.h"

void transmit::transminit(const char* ip)
{
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		std::cerr << "Could not create socket" << std::endl;
	} else {
		std::memset((char*)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.s_addr = inet_addr(ip);
	}
}

void int_to_bytes(int n, uint8_t *bytes, int &a) 
{	
   	bytes[a+0] = (n >> 24) & 0xFF;
	bytes[a+1] = (n >> 16) & 0xFF;
	bytes[a+2] = (n >> 8) & 0xFF;
	bytes[a+3] = n & 0xFF;
 	a += 4;
}

double angle_from_px(int object_x) //all angles in radians
{
	return atan((2*object_x - CAMERA_WIDTH) * tan(FOV/2) / CAMERA_WIDTH);
}

void transmit::send(std::vector<int> centre_xs, std::vector<int> distances, const char* message)
{
	uint8_t buff[1024];
	std::memset(buff, 0, 1024);
	int i = 0;

	//send centre_xs 
	int_to_bytes(0xEC5, buff, i);
	std::for_each(centre_xs.begin(), centre_xs.end(), [&buff, &i](int x){int_to_bytes(x, buff, i);});

	//send angles
	int_to_bytes(0xDE6, buff, i);
	std::for_each(centre_xs.begin(), centre_xs.end(), [&buff, &i](int x){int_to_bytes(((angle_from_px(x)+0.436332313)*1024), buff, i);});
	
	//send distances
	int_to_bytes(0xD15, buff, i);
	std::for_each(distances.begin(), distances.end(), [&buff, &i](int x){int_to_bytes(x, buff, i);});
	
	//send message
	int_to_bytes(0xA2C, buff, i);
	int c = 0;
	while (message[c] != '\0') {
		int_to_bytes(message[c], buff, i);
		c++;
	}

	//send EOF
	int_to_bytes(0xE0F, buff, i);
	sendto(sockfd, buff, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
}

void transmit::test()
{
	uint8_t buff[1024];
	std::memset(buff, 0, 1024);
	int a = 0;
	for (int i = 0; i < 256; i++)
		int_to_bytes(i, buff, a);
	sendto(sockfd, buff, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
}
