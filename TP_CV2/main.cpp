#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void main() {
	cv::Mat img = cv::imread("Resources/black_white.jpg", 0);
	cv::imshow("window", img);


	// 1
	cv::Mat greyX, greyX8, greyY, greyY8, greyXY, greyXY8;

	cv::Sobel(img, greyX, CV_16S, 1, 0);
	greyX.convertTo(greyX8, CV_8U, 0.5, 128);
	cv::Sobel(img, greyY, CV_16S, 0, 1);
	greyY.convertTo(greyY8, CV_8U, 0.5, 128);
	cv::Sobel(img, greyXY, CV_16S, 1, 1);
	greyXY.convertTo(greyXY8, CV_8U, 0.5, 128);

	cv::imshow("greyX8", greyX8);
	cv::imshow("greyY8", greyY8);
	cv::imshow("greyXY8", greyXY8);

	// 2
	//cv::Mat canny, blurred_img, blurred_canny;
	//cv::Canny(img, canny, 50, 100);
	//cv::imshow("canny", canny);

	//cv::blur(img, blurred_img, cv::Size(5, 5));
	//cv::Canny(blurred_img, blurred_canny, 50, 100);
	//cv::imshow("blurred_canny", blurred_canny);

	//// 3
	//cv::Mat display, displayy;
	//cv::Scalar scalar(255, 0, 0);
	//cv::cvtColor(img, display, CV_BGR2HSV);
	//displayy.setTo(display, blurred_canny);
	//cv::imshow("displayy", displayy);

	cv::waitKey(0);
}