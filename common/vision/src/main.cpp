#include <iostream>
#include <thread>
#include <functional>

#include "capture.h"
#include "processing.h"
#include "display.h"

capture cap;
processing proc;
display disp;

int main (int argc, char* argv[])
{
	cap.init();

	std::thread cap_thread([=]() {
				while (true) {
					cap.run();
				}
			});
	cap_thread.detach();

	std::thread proc_thread([=]() {
				while (true) {
					cv::Mat *recv = cap.get_frame();
					if (recv != nullptr && !recv->empty()) {
						cv::Mat tmp = recv->clone();
						proc.process_frame(&tmp);
					}
				}
			});

	proc_thread.detach();

	std::thread disp_thread([=]() {
				while (true) {
					disp.put(cap.get_frame(), proc.get_thresholded(), proc.get_filtered_contours(), proc.get_bounding_boxes(), proc.get_centre_xs());
				}
			});
	disp_thread.join();


	return 0;
}
