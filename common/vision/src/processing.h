#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>

class processing {
	public:
		void process_frame(cv::Mat* frame);
		cv::Mat* get_thresholded();
		std::vector< std::vector<cv::Point> >* get_filtered_contours();
		std::vector< std::vector<cv::Point> >* get_bounding_boxes();
		std::vector<int>* get_centre_xs();

	private:
		cv::Mat thresholded;
		std::vector< std::vector<cv::Point> > filtered_contours;
		std::vector< std::vector<cv::Point> > bounding_boxes;
		std::vector<int> centre_xs;
		std::mutex thresholded_mtx, filtered_contours_mtx, bounding_boxes_mtx, centre_xs_mtx;
};
