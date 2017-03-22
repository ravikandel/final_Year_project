//
//#include<opencv\highgui.h>
//#include<opencv2\opencv.hpp>
//using namespace cv;
//
//void main() {
//	Mat myImage;
//	Mat grayImage;
//	Mat hsvImage;
//	Mat thresholdImage;
//	VideoCapture myCapture;
//	myCapture.open(0);
//
//	for (;;)
//	{
//		myCapture >> myImage;
//		imshow("My_Window", myImage);
//		waitKey(10);
//
//		cvtColor(myImage, grayImage, CV_RGB2GRAY);
//
//		cvtColor(myImage, hsvImage, CV_BGR2HSV);
//
//		inRange(hsvImage, Scalar(10, 20, 30), Scalar(110, 115, 140), thresholdImage);
//		imshow("threshold", thresholdImage);
//
//	}
//}