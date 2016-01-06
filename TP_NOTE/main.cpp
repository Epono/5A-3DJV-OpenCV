#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/calib3d.hpp>

void findMatchings(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2);
void showMatchings(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2);
void rectify(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2, cv::Mat &rectified1, cv::Mat &rectified2);
cv::Mat computeDisparity(cv::Mat &rectified1, cv::Mat &rectified2);


int main() {
	cv::Mat image1 = cv::imread("Resources/left.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2 = cv::imread("Resources/right.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	std::vector<cv::Point2f> points1;
	std::vector<cv::Point2f> points2;
	findMatchings(image1, image2, points1, points2);
	findMatchings(image2, image1, points2, points1);
	showMatchings(image1, image2, points1, points2);
	cv::Mat rectified1(image1.size(), image1.type());
	cv::Mat rectified2(image2.size(), image2.type());
	rectify(image1, image2, points1, points2, rectified1, rectified2);
	cv::imshow("rectified L", rectified1);
	cv::imshow("rectified R", rectified2);
	cv::waitKey();
	cv::Mat disparity = computeDisparity(rectified1, rectified2);
	cv::imshow("disparity", disparity);
	cv::waitKey();
	return 0;
}

void findMatchings(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2) {
	std::vector<cv::Point2f> tmpA;
	cv::goodFeaturesToTrack(image1, tmpA, 200, 0.01, 20);

	std::vector<unsigned char> status;
	std::vector<float> err;
	std::vector<cv::Point2f> tmpB;
	cv::calcOpticalFlowPyrLK(image1, image2, tmpA, tmpB, status, err);

	for(int i = 0; i < tmpA.size(); ++i) {
		if(status[i] == 1) {
			points1.push_back(tmpA[i]);
			points2.push_back(tmpB[i]);
		}
	}
}

void showMatchings(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2) {
	cv::Mat matchings;
	image1.convertTo(matchings, CV_8UC4, 1, 0);

	for(int i = 0; i < points1.size(); ++i) {
		cv::line(matchings, points1[i], points2[i], cv::Scalar(0.f, 0.f, 1.f));
	}
	cv::imshow("matchings", matchings);

	cv::Mat matchingsBig(image1.rows, image1.cols + image2.cols, image1.type()), out;
	image1.copyTo(matchingsBig(cv::Rect(0, 0, image1.cols, image1.rows)));
	image2.copyTo(matchingsBig(cv::Rect(image1.cols, 0, image2.cols, image2.rows)));

	cv::cvtColor(matchingsBig, out, CV_GRAY2BGR);

	for(int i = 0; i < points1.size(); ++i) {
		cv::line(out, points1[i], cv::Point2f(points2[i].x + image1.cols, points2[i].y), cv::Scalar(0.f, 0.f, 1.f));
	}
	cv::imshow("matchingsBig", out);
}

void rectify(cv::Mat &image1, cv::Mat &image2, std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2, cv::Mat &rectified1, cv::Mat &rectified2) {
	cv::Mat fundalmentalMat = cv::findFundamentalMat(points1, points2);

	cv::Mat hl, hr;
	bool boolean = cv::stereoRectifyUncalibrated(points1, points2, fundalmentalMat, image1.size(), hl, hr);

	cv::warpPerspective(image1, rectified1, hl, image1.size());
	cv::warpPerspective(image2, rectified2, hr, image2.size());
}

cv::Mat computeDisparity(cv::Mat &rectified1, cv::Mat &rectified2) {
	cv::Mat disparity16S = cv::Mat(rectified1.rows, rectified1.cols, CV_16S);
	cv::Mat disparity8U = cv::Mat(rectified2.rows, rectified2.cols, CV_8UC1);

	cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(80, 21);
	sbm->compute(rectified1, rectified2, disparity16S);

	double min; double max;
	cv::minMaxLoc(disparity16S, &min, &max);

	disparity16S.convertTo(disparity8U, CV_8UC1, 255 / (max - min), -min * 255 / (max - min));

	return disparity8U;
}