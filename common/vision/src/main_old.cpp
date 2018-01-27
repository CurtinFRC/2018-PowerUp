#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

const int CAMERA_WIDTH = 640, CAMERA_HEIGHT = 480;

std::vector<cv::Point> bounding_box(std::vector<cv::Point> contour)
{
	int x_max = 0, x_min = CAMERA_WIDTH, y_max = 0, y_min = CAMERA_HEIGHT;
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
	int x_max = 0, x_min = CAMERA_WIDTH;
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
	int y_max = 0, y_min = CAMERA_HEIGHT;
	for (cv::Point point : contour)
	{
		if (point.y > y_max)
			y_max = point.y;
		if (point.y < y_min)
			y_min = point.y;
	}

	return (y_min + y_max) / 2;
}

void draw_crosshair(cv::Mat mat, cv::Point p, int length, cv::Scalar color)
{
	line(mat, { p.x, p.y - length / 2 }, { p.x, p.y + length / 2 }, color);
	line(mat, { p.x - length / 2, p.y }, { p.x + length / 2, p.y }, color);
}

int main()
{
	cv::VideoCapture cam(0);
	if (!cam.isOpened())
	{
		std::cerr << "camera not found" << std::endl;
		return -1;
	}

	cam.set(CV_CAP_PROP_BUFFERSIZE, 3);
	cam.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);

	cv::namedWindow("Raw frames", 1);
	cv::namedWindow("Threshold", 1);
	cv::namedWindow("Contours", 1);

	auto last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

	while (true)
	{
		cv::Mat frame_bgr, frame_hsv, filtered_contours_mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC3);
		cam >> frame_bgr;
		cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);

		//HSV threshold
		cv::Mat thresholded;
		cv::inRange(frame_hsv, cv::Scalar(15, 67, 34), cv::Scalar(46, 205, 255), thresholded);

		//find contours
		std::vector< std::vector<cv::Point> > contours;
		cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);

		//filter contours
		std::vector< std::vector<cv::Point> > filtered_contours;
		for (int i = 0; i < contours.size(); i++)
		{
			double area = cv::contourArea(contours[i], false);
			if (area >= 2500)
			{
				filtered_contours.push_back(contours[i]);
				cv::drawContours(filtered_contours_mat, filtered_contours, filtered_contours.size() - 1, cv::Scalar(0xFF, 0, 0));
			}
		}

		//create bounding boxes and crosshair
		std::vector< std::vector<cv::Point> > bounding_boxes;
		for (auto contour : filtered_contours)
		{
			bounding_boxes.push_back(bounding_box(contour));
			cv::drawContours(filtered_contours_mat, bounding_boxes, bounding_boxes.size() - 1, cv::Scalar(0, 0, 0xFF));
			cv::Point center = { center_x(bounding_boxes[bounding_boxes.size() - 1]), center_y(bounding_boxes[bounding_boxes.size() - 1]) };
			draw_crosshair(filtered_contours_mat, center, 20, cv::Scalar(0xFF, 0xFF, 0xFF));
		}
		//join nearby contours
		

		//fps
		auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		int ms = (time - last_time).count();
		double fps = 1000 / (double)ms;
		last_time = time;

		//present graphically
		putText(filtered_contours_mat, "Frames per second: " + std::to_string(fps), { 5, 30 }, cv::FONT_HERSHEY_SCRIPT_COMPLEX, 0.7, { 0x70, fps*fps + 100, 0x70 });
		line(filtered_contours_mat, { CAMERA_WIDTH / 2, 0 }, { CAMERA_WIDTH / 2, CAMERA_HEIGHT }, cv::Scalar(0xFF, 0xFF, 0xFF), 1, 8);
		cv::imshow("Raw frames", frame_bgr);
		cv::imshow("Threshold", thresholded);
		cv::imshow("Contours", filtered_contours_mat);

		cv::waitKey(10);
	}

	return 0;
}
