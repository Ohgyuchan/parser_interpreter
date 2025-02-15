#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	Mat img = imread("lane.png");

	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	Mat img_canny;
	Canny(img_gray, img_canny, 150, 255);

	vector<Vec2f> lines;
	HoughLines(img_canny, lines, 1, CV_PI / 180, 150);

	Mat img_hough;
	img.copyTo(img_hough);
	
	Mat img_lane;
	threshold(img_canny, img_lane, 150, 255, THRESH_MASK);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(img_hough, pt1, pt2, Scalar(0,0,255), 2, 8);
		line(img_lane, pt1, pt2, Scalar::all(255), 1, 8);
	}

	imshow("img_canny", img_hough);
	imshow("img_lane", img_lane);

	waitKey(0);
	return 0;
}