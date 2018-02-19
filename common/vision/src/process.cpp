#include "process.h"

double pythag (double a, double b)
{
	return sqrt(a*a + b*b);
}
double distance_between_contours(std::vector<cv::Point> &contour1, std::vector<cv::Point> &contour2)
{
	if (contour1.size() == 4 && contour1.size() == 4) {
		int x_gap, y_gap;
		if ((contour1[0].x <= contour2[0].x && contour2[0].x <= contour1[1].x) || (contour1[0].x <= contour2[1].x && contour2[1].x <= contour1[1].x)) {
			x_gap = 0;
		} else {
			x_gap = std::min(abs(contour1[0].x - contour2[1].x), abs(contour2[0].x - contour1[1].x));
		}
		if ((contour1[0].y <= contour2[0].y && contour2[0].y <= contour1[2].y) || (contour1[0].y <= contour2[2].y && contour2[2].y <= contour1[2].y)) {
			y_gap = 0;
		} else {
			y_gap = std::min(abs(contour1[0].y - contour2[2].y), abs(contour2[0].y - contour1[2].y));
		}

		return sqrt(x_gap*x_gap + y_gap*y_gap);
	} else {
		return 1234567890;
	}
}

int contour_height(std::vector<cv::Point> contour)
{
	int y_max = 0, y_min = INT_MAX;
	for (cv::Point point : contour)
	{
		if (point.y > y_max)
			y_max = point.y;
		if (point.y < y_min)
			y_min = point.y;
	}

	return y_max - y_min;
}

std::vector<cv::Point> bounding_box(std::vector<cv::Point> &contour)
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

int center_x(std::vector<cv::Point> &contour)
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

int center_y(std::vector<cv::Point> &contour)
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

double distance_from_height(int height, int actual_height)
{
	const double focal_constant = 1.530;

	return focal_constant * actual_height * CAMERA_HEIGHT / height;
}

void process::process_frame(cv::Mat* frame)
{
	const double MERGE_THRESHOLD = 0.050;

	all_mtx.lock();
	//HSV threshold
	cv::Mat frame_hsv;
	cvtColor(*frame, frame_hsv, cv::COLOR_BGR2HSV);
	thresholded_mtx.lock();
	cv::inRange(frame_hsv, cv::Scalar(15, 67, 34), cv::Scalar(46, 205, 255), thresholded);

	//find contours
	std::vector< std::vector<cv::Point> > contours;
	cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
	thresholded_mtx.unlock();

	//filter contours
	filtered_contours_mtx.lock();
	filtered_contours.clear();
	for (int i = 0; i < contours.size(); i++) {
		double area = cv::contourArea(contours[i], false);
		if (area >= 1200)
		{
			filtered_contours.push_back(contours[i]);
		}
	}

	//find bounding boxes
	bounding_boxes_mtx.lock();
	bounding_boxes.clear();
	for (auto contour : filtered_contours) {
		bounding_boxes.push_back(bounding_box(contour));
	}
	filtered_contours_mtx.unlock();

	//merge bounding boxes
	for (int i = 0; i < bounding_boxes.size(); i++) {
		for (int j = 0; j < bounding_boxes.size(); j++) {
			if (i != j && distance_between_contours(bounding_boxes[i], bounding_boxes[j]) < MERGE_THRESHOLD*std::max(contour_height(bounding_boxes[i]), contour_height(bounding_boxes[j]))) {
				bounding_boxes[i].insert(bounding_boxes[i].end(), bounding_boxes[j].begin(), bounding_boxes[j].end());
				bounding_boxes[i] = bounding_box(bounding_boxes[i]);
				bounding_boxes.erase(bounding_boxes.begin()+j);
			}
		}
	}

	//bounding boxxes to centre_xs
	centre_xs_mtx.lock();
	centre_xs.clear();
	std::sort(bounding_boxes.begin(), bounding_boxes.end(), [](std::vector<cv::Point> a, std::vector<cv::Point> b){return contour_height(a) > contour_height(b);});
	std::transform(bounding_boxes.begin(), bounding_boxes.end(), std::back_inserter(centre_xs), center_x);
	centre_xs_mtx.unlock();
	
	//find distances assuming 11in height
	distances_mtx.lock();
	distances.clear();
	std::transform(bounding_boxes.begin(), bounding_boxes.end(), std::back_inserter(distances), [](std::vector<cv::Point> v){ return distance_from_height(contour_height(v), 279);} );
	distances_mtx.unlock();
	bounding_boxes_mtx.unlock();
	all_mtx.unlock();
}

cv::Mat* process::get_thresholded()
{
	std::lock_guard<std::mutex> lock_guard(thresholded_mtx);
	return &thresholded;
}

std::vector< std::vector<cv::Point> >* process::get_filtered_contours()
{
	std::lock_guard<std::mutex> lock_guard(filtered_contours_mtx);
	return &filtered_contours;
}

std::vector< std::vector<cv::Point> >* process::get_bounding_boxes()
{
	std::lock_guard<std::mutex> lock_guard(bounding_boxes_mtx);
	return &bounding_boxes;
}

std::vector<int>* process::get_distances()
{
	std::lock_guard<std::mutex> lock_guard(distances_mtx);
	return &distances;
}

std::vector<int>* process::get_centre_xs()
{
	std::lock_guard<std::mutex> lock_guard(centre_xs_mtx);
	return &centre_xs;
}

std::unique_lock<std::mutex>* process::cv_wait()
{
	u_lock = new std::unique_lock<std::mutex>(all_mtx);
	processed_cv.wait(*u_lock);

	return u_lock;
}
