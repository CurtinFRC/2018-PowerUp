#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

class display 
{
	public:
		void init();
		void put(cv::Mat* frame, cv::Mat* thresholded, std::vector< std::vector<cv::Point> >* filtered_contours, std::vector< std::vector<cv::Point> >* bounding_boxes, std::vector<int>* centre_xs);
};
