#include "capture.h"

void capture::init()
{
	video_capture = new cv::VideoCapture(0);
	if (!video_capture->isOpened()) {
		std::cerr << "Could not open camera." << std::endl;
		exit(-1);
	}

	//video_capture->set(CV_CAP_PROP_BUFFERSIZE, 3);
	video_capture->set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
	video_capture->set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
}

void capture::run()
{
	frame_mtx.lock();
	(*video_capture) >> frame;
	frame_mtx.unlock();
	cv::waitKey(10);
}

cv::Mat* capture::get_frame()
{
	std::lock_guard<std::mutex> lock_guard(frame_mtx);
	return &frame;
}
