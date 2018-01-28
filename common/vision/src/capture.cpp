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
	cap_cv.notify_all();
	cv::waitKey(10);
}

cv::Mat* capture::get_frame()
{
	return &frame;
}

void capture::lock() {
	frame_mtx.lock();
}

void capture::unlock() {
	frame_mtx.unlock();
}

std::shared_ptr<std::unique_lock<std::mutex> > capture::cv_wait()
{
	auto u_lock = std::make_shared<std::unique_lock<std::mutex> >(frame_mtx);
	cap_cv.wait(*u_lock);

	return u_lock;
}
