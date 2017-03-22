//#include<iostream>
//#include<cmath>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<opencv2/highgui/highgui.hpp>
//
//using namespace std;
//using namespace cv;
//
//
//// Computes the x component of the gradient vector
//// at a given point in a image.
//// returns gradient in the x direction
//int Gauss(Mat image, int x, int y)
//{
//	/*return image.at<uchar>(y - 1, x - 1) +
//		2 * image.at<uchar>(y, x - 1) +
//		image.at<uchar>(y + 1, x - 1) -
//		image.at<uchar>(y - 1, x + 1) -
//		2 * image.at<uchar>(y, x + 1) -
//		image.at<uchar>(y + 1, x + 1);*/
//
//	/*return ((1.0/16.0)*(image.at<uchar>(y+1,x-1)+
//		2* image.at<uchar>(y+1,x)+
//		image.at<uchar>(y+1,x+1)+
//		2* image.at<uchar>(y,x-1)+
//		4* image.at<uchar>(y,x)+
//		2* image.at<uchar>(y,x+1)+
//		image.at<uchar>(y-1,x-1)+
//		2* image.at<uchar>(y-1,x)+
//		image.at<uchar>(y-1,x+1)));*/
//	return ((1.0 / 64.0)*(
//		4 *image.at<uchar>(y + 1, x - 1) +
//		8 * image.at<uchar>(y + 1, x) +
//		4* image.at<uchar>(y + 1, x + 1) +
//		8 * image.at<uchar>(y, x - 1) +
//		16 * image.at<uchar>(y, x) +
//		8 * image.at<uchar>(y, x + 1) +
//		4*image.at<uchar>(y - 1, x - 1) +
//		8 * image.at<uchar>(y - 1, x) +
//		image.at<uchar>(y - 1, x + 1)));
//}
//
//
//
//int main()
//{
//
//	Mat src, dst;
//	int gx,x=0,y=0;
//
//	// Load an image
//	src = imread("recharge.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	//src = imread("mm.jpg", 1);
//	dst = src.clone();
//	if (!src.data)
//	{
//		return -1;
//	}
//
//
//	for (y = 0; y < src.rows; y++)
//		for (x = 0; x < src.cols; x++)
//			dst.at<uchar>(y, x) = 0.0;
//
//	for (y = 1; y < src.rows - 1; y++) {
//		for (x = 1; x < src.cols - 1; x++) {
//			gx = Gauss(src, x, y);
//			gx = gx > 255 ? 255 : gx;
//			gx = gx < 0 ? 0 : gx;
//			dst.at<uchar>(y, x) = gx;
//			
//		}
//		
//	}
//
//	namedWindow("final");
//	imshow("final", dst);
//
//	namedWindow("initial");
//	imshow("initial", src);
//
//	cout << y << endl << x;
//	waitKey();
//
//
//	return 0;
//}