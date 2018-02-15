#include <iostream>
#include "rioreceive.h"

int main()
{
	rioreceive rr;
	rr.init();
	std::cout << "Pseudorio started" << std::endl;

	while (true) {
		std::vector<int> centre_xs;
		std::vector<double> angles, distances;
		std::string message;

		rr.get(centre_xs, angles, distances, message);

		for (int n = 0; n < centre_xs.size(); n++)
		{
			std::cout << distances[n] << " at " << angles[n]*180/3.14 << '\t';
		}
		std::cout << centre_xs.size() << std::endl;
	}
	return 0;
}
