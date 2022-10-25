#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#include <time.h>
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

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {
	clock_t start_g, end_g, start_c, end_c;
	double result_g, result_c;

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
	
	start_g = clock();
	output_gray = gaussianfilter(input_gray, 5,2,2, "zero-paddle"); //Boundary process: zero-paddle, mirroring, adjustkernel
	end_g = clock();
	result_g = (double)(end_g - start_g);
	std::cout << result_g << std::endl;

	namedWindow("Gaussian Filter(Gray)", WINDOW_AUTOSIZE);
	imshow("Gaussian Filter(Gray)", output_gray);

	namedWindow("Color", WINDOW_AUTOSIZE);
	imshow("Color", input);
	start_c = clock();
	output = gaussianfilter(input, 5, 2, 2, "zero-paddle"); //Boundary process: zero-paddle, mirroring, adjustkernel
	end_c = clock();
	result_c = (double)(end_c - start_c);
	std::cout << result_c << std::endl;
	namedWindow("Gaussian Filter(Color)", WINDOW_AUTOSIZE);
	imshow("Gaussian Filter(Color)", output);


	waitKey(0);

	return 0;
}


Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel;
	Mat W_s;
	Mat W_t;
	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom_s;
	float denom_t;
	float kernelvalue;

 // Initialiazing Kernel Matrix 
	W_s = Mat::zeros(kernel_size, 1, CV_32FC1);
	W_t = Mat::zeros(1, kernel_size, CV_32FC1);
	kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
	
	denom_s = 0.0;
	denom_t=0.0;

	for (int a = -n; a <= n; a++)
	{
		float value_s = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
		float value_t = exp(-(pow(a, 2) / (2 * pow(sigmaT, 2))));
		W_s.at<float>(a + n,0) = value_s;
		W_t.at<float>(0, a + n) = value_t;
		denom_s += value_s;
		denom_t += value_t;
	}

	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		W_s.at<float>(a+n, 0) /= denom_s;
		W_t.at<float>(0, a+n) /= denom_t;

	}

	Mat temp = Mat::zeros(row, col, input.type());
	Mat output = Mat::zeros(row, col, input.type());
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!strcmp(opt, "zero-paddle")) {
				if (input.channels() == 1)
				{
					float sum1 = 0.0;
					for (int b = -n; b <= n; b++)
						if ((j + b <= col - 1) && (j + b >= 0)) {
							//if the pixel is not a border pixel
							sum1 += W_t.at<float>(0, b + n) * (float)(input.at<G>(i, j + b));
						}
					temp.at<G>(i, j) = (G)sum1;
				}
				else if (input.channels() == 3)
				{
					float sum1_r = 0.0;
					float sum1_g = 0.0;
					float sum1_b = 0.0;
					
					for (int b = -n; b <= n; b++) {
						if ((j + b <= col - 1) && (j + b >= 0)) {
							//if the pixel is not a border pixel
							sum1_r += W_t.at<float>(0, b + n) * (float)(input.at<C>(i , j + b)[0]);
							sum1_g += W_t.at<float>(0, b + n) * (float)(input.at<C>(i, j + b)[1]);
							sum1_b += W_t.at<float>(0, b + n) * (float)(input.at<C>(i, j + b)[2]);
						}
					}
					
					temp.at<C>(i, j)[0] = (G)sum1_r;
					temp.at<C>(i, j)[1] = (G)sum1_g;
					temp.at<C>(i, j)[2] = (G)sum1_b;
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

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!strcmp(opt, "zero-paddle")) {
				if (input.channels() == 1)
				{
					float sum1 = 0.0;
					for (int a = -n; a <= n; a++)
						if ((i + a <= row - 1) && (i + a >= 0)) {
							//if the pixel is not a border pixel
							sum1 += W_s.at<float>(a + n, 0) * (float)(temp.at<G>(i + a, j));
						}
					output.at<G>(i, j) = (G)sum1;
				}
				else if (input.channels() == 3)
				{
					float sum1_r = 0.0;
					float sum1_g = 0.0;
					float sum1_b = 0.0;

					for (int a = -n; a <= n; a++) {
						if ((i + a <= row - 1) && (i + a >= 0)) {
							//if the pixel is not a border pixel
							sum1_r += W_s.at<float>(a + n, 0) * (float)(temp.at<C>(i + a, j)[0]);
							sum1_g += W_s.at<float>(a + n, 0) * (float)(temp.at<C>(i + a, j)[1]);
							sum1_b += W_s.at<float>(a + n, 0) * (float)(temp.at<C>(i + a, j)[2]);
						}
					}

					output.at<C>(i, j)[0] = (G)sum1_r;
					output.at<C>(i, j)[1] = (G)sum1_g;
					output.at<C>(i, j)[2] = (G)sum1_b;
				}
			}
		}
	}
	return output;
}