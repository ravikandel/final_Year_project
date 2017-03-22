//#include <iostream>
//#include <vector>
//#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//using namespace std;
//using namespace cv;
//
//int main(int argc, char* argv[])
//{
//	//cv::Mat input;
//	//cv::VideoCapture myCapture; // FOR CAMERA INPUT
//	//myCapture.open(0); // FOR CAMERA INPUT
//
//	//for (;;)
//	//{
//		//myCapture >> input; // FOR CAMERA INPUT
//		//Load input image (colored, 3-channel)
//		Mat input = imread("recharge.jpg");
//		if (input.empty())
//		{
//			cout << "!!! failed imread()" << endl;
//			return -1;
//		}
//
//		//Convert input image to grayscale (1-channel)
//		Mat grayscale = input.clone();
//		cvtColor(input, grayscale, COLOR_BGR2GRAY);
//		//cv::imwrite("gray.png", grayscale);
//
//		//Threshold to binarize the image and get rid of the shoe
//		Mat binary;
//		threshold(grayscale, binary, 225, 255, THRESH_BINARY_INV);
//		imshow("Binary Image", binary);
//		//cv::imwrite("binary.png", binary);
//
//		// Wait until user presses key
//		waitKey(0);
//
//	//}
//
//	return 0;
//
//}