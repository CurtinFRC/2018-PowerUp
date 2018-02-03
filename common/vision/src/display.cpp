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

template <class type>
void print_vector(std::vector<type> v)
{
	std::cout << "<";
	for (type i : v)
		std::cout << " " << i;
	std::cout << " >" << std::endl;
}

void display::init()
{
	cv::namedWindow("Raw", 1);
	cv::namedWindow("Threshold", 1);
	cv::namedWindow("Processed", 1);
}

void display::put(cv::Mat* frame, cv::Mat* thresholded, std::vector< std::vector<cv::Point> >* filtered_contours, std::vector< std::vector<cv::Point> >* bounding_boxes, std::vector<int>* centre_xs)
{
	if (!frame->empty())
		cv::imshow("Raw", *frame);
	if (!thresholded->empty())
		cv::imshow("Threshold", *thresholded);
	
	cv::Mat processed(frame->rows, frame->cols, CV_8UC3);
	for (int i = 0; i < filtered_contours->size(); i++)
		cv::drawContours(processed, *filtered_contours, i, cv::Scalar(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < bounding_boxes->size(); i++)
		cv::drawContours(processed, *bounding_boxes, i, cv::Scalar(0xFF, 0, 0xFF));
	for (int i = 0; i < centre_xs->size(); i++)
		draw_vertical_line(&processed, (*centre_xs)[i], 1, cv::Scalar(0, 0, 0xFF));

	cv::imshow("Processed", processed);
}

double average(std::vector<double> v)
{
	double total = 0;
	for (double i : v)
		total += i;

	return total/v.size();
}

void display::text (std::vector< std::vector<cv::Point> >* filtered_contours, std::vector< std::vector<cv::Point> >* bounding_boxes, std::vector<int>* centre_xs, std::vector<double> cam_fps, std::vector<double> proc_fps, std::vector<double> disp_fps)
{
	std::cout << "centre_xs: " << centre_xs->size() << std::endl;
	print_vector(*centre_xs);
	std::cout << "centre_xs length: " << centre_xs->size() << std::endl;
	std::cout << "filter contours length: " << filtered_contours->size() << std::endl;
	std::cout << "bounding boxes length: " << bounding_boxes->size() << std::endl;
	std::cout << "Cap fps: " << average(cam_fps)  << "\tProc fps: " << average(proc_fps) << "\tdisp_fps: " << average(disp_fps) << std::endl;
}
