#include <iostream>
#include <thread>
#include <functional>
#include <unistd.h>

#include "capture.h"
#include "process.h"
#include "display.h"
#include "config.h" 
#include "transmit.h"

capture cap;
process proc;
display disp;
transmit trans;

std::vector<double> cap_fps, proc_fps, disp_fps, trans_fps; 

int main (int argc, char* argv[])
{
	system("./camera-settings.sh");
	system("./camera-settings1.sh");
	if (argc == 2)
		cap.init(atoi(argv[1]));
	else
		cap.init(0);

	std::thread cap_thread([=]() {	
				std::cout << "T(ID) CAP = " << std::this_thread::get_id() << std::endl;
				auto last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				while (true) {
					cap.run();
			
					auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
					int ms = (time - last_time).count();
					cap_fps.insert(cap_fps.begin(), 1000 / (double)ms);
					if (cap_fps.size() > 10)
						cap_fps.pop_back();
		
					last_time = time;
					if (ms < 100)
						usleep(100 - ms);
				}
			});
	cap_thread.detach();

	std::thread proc_thread([=]() {
				std::cout << "T(ID) PROC = " << std::this_thread::get_id() << std::endl;
				auto last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				while (true) {
					auto lock = cap.cv_wait();
					cv::Mat tmp = *cap.get_frame();
					lock->unlock();

					proc.process_frame(&tmp);

					auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
					int ms = (time - last_time).count();
					proc_fps.insert(proc_fps.begin(), 1000 / (double)ms);
					if (proc_fps.size() > 10)
						proc_fps.pop_back();
					last_time = time;
				}
			});

	proc_thread.detach();

	disp.init();
	std::thread disp_thread([=]() {
				std::cout << "T(ID) DISP = " << std::this_thread::get_id() << std::endl;
				auto last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				while (true) 
				{
					auto lock = cap.cv_wait();
					cv::Mat recv = *cap.get_frame();
					//lock->unlock();

					disp.put(&recv, proc.get_thresholded(), proc.get_filtered_contours(), proc.get_bounding_boxes(), proc.get_centre_xs());
					disp.text(proc.get_filtered_contours(), proc.get_bounding_boxes(), proc.get_centre_xs(), cap_fps, proc_fps, disp_fps);
					auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
					int ms = (time - last_time).count();
					disp_fps.insert(disp_fps.begin(), 1000 / (double)ms);
					if (disp_fps.size() > 10)
						disp_fps.pop_back();
					last_time = time;
					if (ms < 50)
						cv::waitKey(50 - ms);
				}
			});
	disp_thread.detach();


	trans.transminit(argv[2]);//"192.168.0.101");//===================================================== change ip =============================================
	std::thread trans_thread([=]() {
				std::cout << "T(ID) TRANS = " << std::this_thread::get_id() << std::endl;
				auto last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				while (true) 
				{
					trans.send(*proc.get_centre_xs(), *proc.get_distances(), "regular");
					//trans.test();

					auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
					int ms = (time - last_time).count();
					trans_fps.insert(trans_fps.begin(), 1000 / (double)ms);
					if (trans_fps.size() > 10)
						trans_fps.pop_back();
					last_time = time;
					if (ms < 100)
						usleep(100 - ms);
				}
			});
	trans_thread.join();

	
	return 0;
}
