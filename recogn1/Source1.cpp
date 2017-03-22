#include <iostream>
#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>
using namespace std;

/*angle: helper function.
Finds a cosine of angle between vectors from pt0->pt1 and from pt0->pt2.*/
double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/*findSquares: returns sequence of squares detected on the image.
The sequence is stored in the specified memory storage.*/
void findSquares(const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares)
{
	cv::Mat pyr, timg;

	//Down-scale and up-scale the image to filter out small noises
	cv::pyrDown(image, pyr);
	cv::pyrUp(pyr, timg, image.size());

	//Apply Canny with a threshold of 50
	cv::Canny(timg, timg, 0, 50, 5);

	//Dilate canny output to remove potential holes between edge segments
	cv::dilate(timg, timg, cv::Mat(), cv::Point(-1, -1));

	//find contours and store them all as a list 
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(timg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++) // Test each contour
	{
		//Approximate contour with accuracy proportional to the contour perimeter
		std::vector<cv::Point> approx;
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		/*Square contours should have 4 vertices after approximation
		relatively large area (to filter out noisy contours)
		and be convex.
		Note: absolute value of an area is used because
		area may be positive or negative - in accordance with the
		contour orientation*/
		if (approx.size() == 4 &&
			fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
			cv::isContourConvex(cv::Mat(approx)))
		{
			double maxCosine = 0;

			for (int j = 2; j < 5; j++)
			{
				//Find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}

			/*If cosines of all angles are small
			(all angles are ~90 degree) then write quandrange
			vertices to resultant sequence*/
			if (maxCosine < 0.3)
				squares.push_back(approx);
		}
	}
}


//drawSquares: function draws all the squares found in the image
void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const cv::Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 2, CV_AA);
	}

	cv::imshow("Final Result", image);
}


int main(int argc, char* argv[])
{
	cv::Mat input, histo;
	double large = 0.0, area = 0.0;
	//cv::VideoCapture myCapture; // FOR CAMERA INPUT
	//myCapture.open(0); // FOR CAMERA INPUT


		//myCapture >> input; // FOR CAMERA INPUT
		 //Load input image (colored, 3-channel)
	input = cv::imread("recharge.jpg");
	histo = input.clone();
	
	
	if (input.empty())
	{
		cout << "!!! failed imread()" << endl;
		return -1;
	}

	//Convert input image to grayscale (1-channel)
	cv::Mat grayscale = input.clone();
	cv::cvtColor(input, grayscale, cv::COLOR_BGR2GRAY);
	cv::imwrite("gray.png", grayscale);

	//Threshold to binarize the image and get rid of the shoe
	cv::Mat binary;
	cv::threshold(grayscale, binary, 225, 255, cv::THRESH_BINARY_INV);
	cv::imshow("Binary Image", binary);
	cv::imwrite("binary.png", binary);
	cv::Mat img1 = binary.clone();

	//Find the contours in the thresholded image
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(binary, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);


	//Fill the areas of the contours with BLUE (hoping to erase everything inside a rectangular shape)
	cv::Mat blue = input.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		std::vector<cv::Point> cnt = contours[i];
		area = cv::contourArea(cv::Mat(cnt));

		if (area > large)
		{
			large = area;
		}

		std::cout << "* Area: " << area << std::endl;
		cv::drawContours(blue, contours, i, cv::Scalar(255, 0, 0),
			CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
	}
	std::cout << "Largest area: " << large;

	cv::imshow("Countours Filled Image", blue);
	cv::imwrite("contours.png", blue);

	//Convert the blue colored image to binary (again), and we will have a good rectangular shape to detect
	cv::Mat gray;
	cv::cvtColor(blue, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 225, 255, cv::THRESH_BINARY_INV);
	cv::imshow("Binary Image 2", binary);
	cv::imwrite("binary2.png", binary);

	//Erode & Dilate to isolate segments connected to nearby areas
	int erosion_type = cv::MORPH_RECT;
	int erosion_size = 5;
	cv::Mat element = cv::getStructuringElement(erosion_type,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::erode(binary, binary, element);
	cv::dilate(binary, binary, element);
	cv::imshow("Morphologic OutPut", binary);
	cv::imwrite("morpho.png", binary);

	//Ok, let's go ahead and try to detect all rectangular shapes
	std::vector<std::vector<cv::Point> > squares;
	findSquares(img1, squares);
	std::cout << "* Rectangular shapes found: " << squares.size() << std::endl;

	//Draw all rectangular shapes found
	cv::Mat output = input.clone();
	drawSquares(output, squares);
	cv::imwrite("output.png", output);


	//Crop the rectangular shape
	if (squares.size() >= 1)
	{
		cv::Rect box;
		if (squares.size() == 1)
		{
			box = cv::boundingRect(cv::Mat(squares[0]));
		}
		else if (squares.size() == 2)
		{
			box = cv::boundingRect(cv::Mat(squares[1]));
		}
		else
		{
			box = cv::boundingRect(cv::Mat(squares[2]));
		}

		cout << "* The location of the box is x:" << box.x << " y:" << box.y << " " << box.width << "x" << box.height << endl;

		// Crop the original image to the defined ROI
		cv::Mat crop = input(box);
		cv::imshow("crop", crop);
		cv::imwrite("cropped.tiff", crop);
		cv::Mat crop1;
		cv::threshold(crop, crop1, 200, 255, CV_THRESH_BINARY_INV);
		cv::imshow("crop1", crop1);
		cv::imwrite("Cropped_Threshold.tiff", crop1);
	}
	else
	{
		cout << "* Abort! More than one rectangle was found." << endl;
	}

	// Wait until user presses key
	cv::waitKey(0);



	return 0;

}