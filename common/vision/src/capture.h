#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>

const int CAMERA_WIDTH = 640, CAMERA_HEIGHT = 480;

class capture 
{
	public:
		void init();
		void run();
		cv::Mat* get_frame();

	private:
		cv::Mat frame;
		cv::VideoCapture* video_capture;
		std::mutex frame_mtx;
};
