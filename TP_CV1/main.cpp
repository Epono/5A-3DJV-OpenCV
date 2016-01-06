#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void main() {
	cv::Mat img = cv::imread("Resources/rgb.png");
	cv::imshow("window", img);

	cv::Mat channel[3];
	split(img, channel);

	cv::imshow("blue", channel[0]);
	cv::imshow("green", channel[1]);
	cv::imshow("red", channel[2]);

	cv::Mat img2 = cv::imread("Resources/texture.jpg");
	cv::Mat bw = img2 > 100;
	cv::imshow("bw", bw);

	cv::Mat gray;
	cv::cvtColor(img2, gray, CV_BGR2GRAY);
	cv::imshow("gray", gray);

	// 4
	cv::Mat grayy = gray > 100;
	std::vector<cv::Mat> channels_t;
	for(auto c = channels_t.begin(); c != channels_t.end(); ++c) {
		channels_t.push_back((*c) > 100);
	}
	cv::Mat merged_t;
	//cv::merge(channels_t, merged_t);
	//cv::imshow("merged_t", merged_t);

	//5
	cv::Mat hsv;
	cv::cvtColor(img, hsv, CV_BGR2HSV);
	std::vector<cv::Mat> hsv_channels(3);
	cv::split(hsv, hsv_channels);
	hsv_channels[0] *= 2;
	cv::merge(hsv_channels, hsv);
	cv::imshow("hsv", hsv);

	cv::waitKey(0);
}