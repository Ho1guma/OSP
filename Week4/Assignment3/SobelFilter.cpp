#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;
	Mat output;
	Mat output_gray;

	cvtColor(input, input_gray, CV_RGB2GRAY);



	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);
	output_gray = sobelfilter(input_gray); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Sobel Filter(gray)", WINDOW_AUTOSIZE);
	imshow("Sobel Filter(gray)", output_gray);

	namedWindow("Color", WINDOW_AUTOSIZE);
	imshow("Color", input);
	output = sobelfilter(input); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Sobel Filter(color)", WINDOW_AUTOSIZE);
	imshow("Sobel Filter(color)", output);


	waitKey(0);

	return 0;
}
Mat sobelfilter(const Mat input) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)

	Mat output = Mat::zeros(row, col, input.type());
	float Sx[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1 } };
	float Sy[3][3] = { {-1,-2,-1},{0,0,0},{1,2,4} };

	
	int tempa;
	int tempb;
	float kernelvalue;
	if (input.channels() == 1)
	{
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				float sum = 0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Use mirroring boundary process
						// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy )

						if (i + a > row - 1) {  //mirroring for the border pixels
							tempa = i - a;
						}
						else if (i + a < 0) {
							tempa = -(i + a);
						}
						else {
							tempa = i + a;
						}
						if (j + b > col - 1) {
							tempb = j - b;
						}
						else if (j + b < 0) {
							tempb = -(j + b);
						}
						else {
							tempb = j + b;
						}
						sum += input.at<G>(tempa, tempb) * Sx[a + n][b + n] +
							input.at<G>(tempa, tempb) * Sy[a + n][b + n];
					}
				}
				sum = sqrt(abs(sum)) * 5;
				output.at<G>(i, j) = (G)sum;
			}
		}
	}
	else if (input.channels() == 3)
	{
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				float sum_r = 0;
				float sum_g = 0;
				float sum_b = 0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Use mirroring boundary process
						// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy )
						if (i + a > row - 1) {  //mirroring for the border pixels
							tempa = i - a;
						}
						else if (i + a < 0) {
							tempa = -(i + a);
						}
						else {
							tempa = i + a;
						}
						if (j + b > col - 1) {
							tempb = j - b;
						}
						else if (j + b < 0) {
							tempb = -(j + b);
						}
						else {
							tempb = j + b;
						}
						sum_r += input.at<C>(tempa, tempb)[0] * Sx[a + n][b + n] +
							input.at<C>(tempa, tempb)[0] * Sy[a + n][b + n];

						sum_g += input.at<C>(tempa, tempb)[1] * Sx[a + n][b + n] +
							input.at<C>(tempa, tempb)[1] * Sy[a + n][b + n];

						sum_b += input.at<C>(tempa, tempb)[2] * Sx[a + n][b + n] +
							input.at<C>(tempa, tempb)[2] * Sy[a + n][b + n];
					}
				}

				sum_r = sqrt(abs(sum_r));
				sum_g = sqrt(abs(sum_g));
				sum_b = sqrt(abs(sum_b));
				output.at<C>(i, j)[0] = (G)((sum_r+sum_g+sum_b)/3*5);
				output.at<C>(i, j)[1] = (G)((sum_r + sum_g + sum_b) / 3*5);
				output.at<C>(i, j)[2] = (G)((sum_r + sum_g + sum_b) / 3*5);
			}
		}
	}
	
	return output;
}