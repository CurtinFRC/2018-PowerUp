#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include "config.h" 

class process
{
	public:
		void process_frame(cv::Mat* frame);
		cv::Mat* get_thresholded();
		std::vector< std::vector<cv::Point> >* get_filtered_contours();
		std::vector< std::vector<cv::Point> >* get_bounding_boxes();
		std::vector<int>* get_distances();
		std::vector<int>* get_centre_xs();

		std::unique_lock<std::mutex>* cv_wait();

	private:
		cv::Mat thresholded;
		std::vector< std::vector<cv::Point> > filtered_contours;
		std::vector< std::vector<cv::Point> > bounding_boxes;
		std::vector<int> centre_xs, distances;
		std::mutex thresholded_mtx, filtered_contours_mtx, bounding_boxes_mtx, centre_xs_mtx, distances_mtx, all_mtx;

		//conditional varable
		std::unique_lock<std::mutex>* u_lock;
		std::condition_variable processed_cv;
};
