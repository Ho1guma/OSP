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

Mat UnsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k);
Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

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
	output_gray = UnsharpMask(input_gray, 5,2,2, "zero-paddle",0.5); //Boundary process: zero-paddle, mirroring, adjustkernel


	namedWindow("UnsharpMask(Gray)", WINDOW_AUTOSIZE);
	imshow("UnsharpMask(Gray)", output_gray);

	namedWindow("Color", WINDOW_AUTOSIZE);
	imshow("Color", input);

	output = UnsharpMask(input, 5, 2, 2, "zero-paddle",0.5); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("UnsharpMask(Color)", WINDOW_AUTOSIZE);
	imshow("UnsharpMask(Color)", output);


	waitKey(0);

	return 0;
}
Mat UnsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k)
{
	Mat L;
	Mat output;
	if (input.channels() == 1)
	{
		L = gaussianfilter(input, n, sigmaT, sigmaS, opt); //blur with low-pass filter
		output = (input - k * L) / (1.0 - k);
	}
	else if (input.channels() == 3)
	{
		L = gaussianfilter(input, n, sigmaT, sigmaS, opt); //blur with low-pass filter
		output = (input - k * L) / (1.0 - k);
	}

	return output;
}

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel;
	
	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom;
	float kernelvalue;

 // Initialiazing Kernel Matrix 
	kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
	
	
	denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<float>(a+n, b+n) = value1;
			denom += value1;
		}
	}

	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {			
			kernel.at<float>(a+n, b+n) /= denom;
		}
	}

	Mat output = Mat::zeros(row, col, input.type());
	
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!strcmp(opt, "zero-paddle")) {
				if (input.channels() == 1)
				{
					float sum1 = 0.0;
					for (int a = -n; a <= n; a++) {
						for (int b = -n; b <= n; b++) {
							if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
								//if the pixel is not a border pixel
								kernelvalue = kernel.at<float>(a + n, b + n);
								sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
							}
						}
					}
					output.at<G>(i, j) = (G)sum1;
				}
				else if (input.channels() == 3)
				{
					float sum1_r = 0.0;
					float sum1_g = 0.0;
					float sum1_b = 0.0;
					for (int a = -n; a <= n; a++) { // for each kernel window
						for (int b = -n; b <= n; b++) {
							if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
								//if the pixel is not a border pixel
								kernelvalue = kernel.at<float>(a + n, b + n);
								sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
								sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
								sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
							}
						}
					}
					output.at<C>(i, j)[0] = (G)sum1_r;
					output.at<C>(i, j)[1] = (G)sum1_g;
					output.at<C>(i, j)[2] = (G)sum1_b;
				}
				
			}
			
			else if (!strcmp(opt, "mirroring")) {
				if (input.channels() == 1)
				{
					float sum1 = 0.0;
					for (int a = -n; a <= n; a++) {
						for (int b = -n; b <= n; b++) {
							kernelvalue = kernel.at<float>(a + n, b + n);
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
							sum1 += kernelvalue * (float)(input.at<G>(tempa, tempb));
						}

					}
					output.at<G>(i, j) = (G)sum1;
				}
				else if (input.channels() == 3)
				{
					float sum1_r = 0.0;
					float sum1_g = 0.0;
					float sum1_b = 0.0;
					for (int a = -n; a <= n; a++) { // for each kernel window
						for (int b = -n; b <= n; b++) {
							kernelvalue = kernel.at<float>(a + n, b + n);
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
							sum1_r += kernelvalue * (float)(input.at<C>(tempa, tempb)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(tempa, tempb)[1]);
							sum1_b += kernelvalue * (float)(input.at<C>(tempa, tempb)[2]);
						}
					}
					output.at<C>(i, j)[0] = (G)sum1_r;
					output.at<C>(i, j)[1] = (G)sum1_g;
					output.at<C>(i, j)[2] = (G)sum1_b;
				}
				
			}


			else if (!strcmp(opt, "adjustkernel")) {
				if (input.channels() == 1)
				{
					float sum1 = 0.0;
					float sum2 = 0.0;
					for (int a = -n; a <= n; a++) { // for each kernel window
						for (int b = -n; b <= n; b++) {
							kernelvalue = kernel.at<float>(a + n, b + n);
							if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
								sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
								sum2 += kernelvalue;
							}
						}
					}
					output.at<G>(i, j) = (G)(sum1 / sum2);
				}
				else if (input.channels() == 3)
				{
					float sum1_r = 0.0;
					float sum1_g = 0.0;
					float sum1_b = 0.0;
					float sum2 = 0.0;
					for (int a = -n; a <= n; a++) { // for each kernel window
						for (int b = -n; b <= n; b++) {
							kernelvalue = kernel.at<float>(a + n, b + n);
							if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
								sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
								sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
								sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
								sum2 += kernelvalue;
							}
						}
					}
					output.at<C>(i, j)[0] = (G)(sum1_r / sum2);
					output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
					output.at<C>(i, j)[2] = (G)(sum1_b / sum2);
				}
			}
		}
	}
	return output;
}