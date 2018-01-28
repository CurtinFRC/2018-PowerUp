#include "processing.h"

std::vector<cv::Point> bounding_box(std::vector<cv::Point> contour)
{
	int x_max = 0, x_min = INT_MAX, y_max = 0, y_min = INT_MAX;
	for (cv::Point point : contour)
	{
		if (point.x > x_max)
			x_max = point.x;
		if (point.y > y_max)
			y_max = point.y;
		if (point.x < x_min)
			x_min = point.x;
		if (point.y < y_min)
			y_min = point.y;
	}

	return { { x_min, y_min },{ x_max, y_min },{ x_max, y_max },{ x_min, y_max } };
}

int center_x(std::vector<cv::Point> contour)
{
	int x_max = 0, x_min = INT_MAX;
	for (cv::Point point : contour)
	{
		if (point.x > x_max)
			x_max = point.x;
		if (point.x < x_min)
			x_min = point.x;
	}

	return (x_min + x_max) / 2;
}

int center_y(std::vector<cv::Point> contour)
{
	int y_max = 0, y_min = INT_MAX;
	for (cv::Point point : contour)
	{
		if (point.y > y_max)
			y_max = point.y;
		if (point.y < y_min)
			y_min = point.y;
	}

	return (y_min + y_max) / 2;
}

void processing::process_frame(cv::Mat* frame)
{
	//all_mtx.lock();
	//HSV threshold
	//cvtColor(*frame, *frame, cv::COLOR_BGR2HSV);
	//thresholded_mtx.lock();
//	cv::inRange(*frame, cv::Scalar(15, 67, 34), cv::Scalar(46, 205, 255), thresholded);

	//find contours
	std::vector< std::vector<cv::Point> > contours;
//	cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
	//thresholded_mtx.unlock();

	//filter contours
	//filtered_contours_mtx.lock();
//	filtered_contours.clear();
//	for (int i = 0; i < contours.size(); i++) {
//		double area = cv::contourArea(contours[i], false);
//		if (area >= 2500)
//		{
//			filtered_contours.push_back(contours[i]);
//		}
//	}

	//find bounding boxes
	//bounding_boxes_mtx.lock();
//	bounding_boxes.clear();
//	for (auto contour : filtered_contours) {
//		bounding_boxes.push_back(bounding_box(contour));
//	}
//	filtered_contours_mtx.unlock();

	//bounding boxxes to centre_xs
	//centre_xs_mtx.lock();
	//centre_xs.clear();
	//std::transform(bounding_boxes.begin(), bounding_boxes.end(), std::back_inserter(centre_xs), center_x);
	//bounding_boxes_mtx.unlock();
	//centre_xs_mtx.unlock();
	//all_mtx.unlock();
}

cv::Mat* processing::get_thresholded()
{
	std::lock_guard<std::mutex> lock_guard(thresholded_mtx);
	return &thresholded;
}

std::vector< std::vector<cv::Point> >* processing::get_filtered_contours()
{
	std::lock_guard<std::mutex> lock_guard(filtered_contours_mtx);
	return &filtered_contours;
}

std::vector< std::vector<cv::Point> >* processing::get_bounding_boxes()
{
	std::lock_guard<std::mutex> lock_guard(bounding_boxes_mtx);
	return &bounding_boxes;
}

std::vector<int>* processing::get_centre_xs()
{
	std::lock_guard<std::mutex> lock_guard(centre_xs_mtx);
	return &centre_xs;
}

std::unique_lock<std::mutex>* processing::cv_wait()
{
	u_lock = new std::unique_lock<std::mutex>(all_mtx);
	processed_cv.wait(*u_lock);

	return u_lock;
}
