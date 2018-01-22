#include "curtinfrc/vision/vision.h"

#include <iostream>
#include <vector>

using namespace cv;
using namespace curtinfrc;

static VisionSystem::tp_millis last_time;

static void _vision_thread_func() {
    cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture(); //__linux__ error
    camera.SetResolution(640, 480);
    camera.SetFPS(30);
    cs::CvSink cvsink = CameraServer::GetInstance()->GetVideo();
    cs::CvSource output_filter = CameraServer::GetInstance()->PutVideo("Filtered", 640, 480);
    cs::CvSource output_src_contours = CameraServer::GetInstance()->PutVideo("Contours", 640, 480);
    cv::Mat source, output, output_contours = Mat::zeros(Size(640, 480), CV_8UC3);

    std::vector<std::vector<Point> > contours, filteredContours;
    contours.reserve(100);
    filteredContours.reserve(100);

    last_time = std::chrono::time_point_cast<std::chrono::milliseconds>(VisionSystem::clock::now());

    while(true) {
        cvsink.GrabFrame(source);
        if (source.empty()) continue;

        Scalar hsv_low(20, 100, 100);		// The lower boundaries of the HSV filter
        Scalar hsv_high(40, 255, 255);	    // The higher boundaries of the HSV filter
        
        cvtColor(source, output, CV_BGR2HSV);		// Convert RGB to HSV
        inRange(output, hsv_low, hsv_high, output);	// Filter values outside of low..high
        output_filter.PutFrame(output);

        findContours(output.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
        for (int i = 0; i < contours.size(); i++) {
            std::vector<Point> contour = contours[i];
            std::vector<Point> hull;

            convexHull(contour, hull);				// Find the contour hull
            double area = contourArea(contour);		// Find the area of the contour
            double solidity = area / contourArea(hull);	// Find the solidity of the contour
            if (area > 1000.0 && solidity > 0.5) {		// Check if the contour meets our conditions
                filteredContours.push_back(contour);	
                drawContours(output_contours, contours, i, Scalar(255, 255, 255));
            }
        }
        output_src_contours.PutFrame(output_contours);


        contours.clear();
        filteredContours.clear();
        output_contours.setTo(Scalar(0, 0, 0));

        // TIME KEEPING //

        auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(VisionSystem::clock::now());
        int ms = (time - last_time).count();
        last_time = time;
        double hz = 1000.0 / static_cast<double>(ms);

        std::cout << "[VISION] " << ms << "ms: " << hz << "hz" << std::endl;
    }
}

void VisionSystem::start() {
    _vision_thread = new std::thread(_vision_thread_func);
    _vision_thread->detach();
}