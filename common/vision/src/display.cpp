#include "display.h"

void draw_crosshair(cv::Mat *mat, cv::Point p, int length, cv::Scalar color)
{
	line(*mat, { p.x, p.y - length / 2 }, { p.x, p.y + length / 2 }, color);
	line(*mat, { p.x - length / 2, p.y }, { p.x + length / 2, p.y }, color);
}

void draw_vertical_line(cv::Mat* mat, int x, int thiccness, cv::Scalar color)
{
	line(*mat, {x, 0}, {x, mat->rows}, color, thiccness);
}

void display::init()
{
	cv::namedWindow("Raw", 1);
	cv::namedWindow("Threshold", 1);
	cv::namedWindow("Processed", 1);
}

void display::put(cv::Mat* frame, cv::Mat* thresholded, std::vector< std::vector<cv::Point> >* filtered_contours, std::vector< std::vector<cv::Point> >* bounding_boxes, std::vector<int>* centre_xs)
{
	cv::imshow("Raw", *frame);
	cv::imshow("Threshold", *thresholded);
	
	cv::Mat processed;
	for (int i = 0; i < filtered_contours->size(); i++)
		cv::drawContours(processed, *filtered_contours, i, cv::Scalar(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < bounding_boxes->size(); i++)
		cv::drawContours(processed, *bounding_boxes, i, cv::Scalar(0xFF, 0, 0xFF));
	for (int i = 0; i < centre_xs->size(); i++)
		draw_vertical_line(&processed, (*centre_xs)[i], 1, cv::Scalar(0, 0, 0xFF));

	cv::imshow("Threshold", *thresholded);

	cv::waitKey(10);
}

