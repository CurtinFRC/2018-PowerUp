#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <condition_variable>
#include <mutex>
#include <memory>

const int CAMERA_WIDTH = 640, CAMERA_HEIGHT = 480;

class capture {
	public:
		void init();
		void run();
		cv::Mat* get_frame();
		void lock();
		void unlock();
		std::shared_ptr<std::unique_lock<std::mutex> > cv_wait();

	private:
		cv::Mat frame;
		cv::VideoCapture* video_capture;
		std::mutex frame_mtx;
		std::condition_variable cap_cv;
};
