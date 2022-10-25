#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE	CV_64FC3

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
Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale
	
	// 8-bit unsigned char -> 64-bit floating point
	input.convertTo(input, CV_64FC3, 1.0 / 255);
	input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

	// Add noise to original image
	Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
	Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

	// Denoise, using gaussian filter
	Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 10, "zero-padding");
	Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10,10, "zero-padding");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (Grayscale)", noise_Gray);

	namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Bilateral Filtering)(Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Bilateral Filtering)(Grayscale)", Denoised_Gray);

	namedWindow("Denoised (Bilateral Filtering)(RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (Bilateral Filtering)(RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {

	Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
	RNG rng;
	rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);

	add(input, NoiseArr, NoiseArr);

	return NoiseArr;
}
Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {
	Mat kernel_spatial;

	int tempa;
	int tempb;

	float kernelvalue;
	float G_sigma_r;
	float G_sigma_s;

	float w_ij = 0.0;
	float w_st = 0.0;

	int kernel_size = (2 * n + 1);
	kernel_spatial = Mat::zeros(kernel_size, kernel_size, CV_32F);

	int row = input.rows;
	int col = input.cols;

	for (int a = -n; a <= n; a++) { 
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2))));
			kernel_spatial.at<float>(a + n, b + n) = value1;
		}
	}
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!strcmp(opt, "zero-padding")) {
				w_st = 0;
				w_ij = 0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							//if the pixel is not a border pixel
							G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
							G_sigma_r = exp(-(pow(input.at<G>(i, j) - input.at<G>(i + a, j + b), 2)) / (2 * pow(sigma_r, 2)));
							kernelvalue = G_sigma_s * G_sigma_r;
							w_ij += kernelvalue;
							w_st += kernelvalue * (float)(input.at<G>(i + a, j + b));
						}
						else
						{
							//0인 값은 삭제 input.at<G>(i + a, j + b)=0
							G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
							G_sigma_r = exp(-(pow(input.at<G>(i, j), 2)) / (2 * pow(sigma_r, 2)));
							kernelvalue = G_sigma_s * G_sigma_r;
							w_ij += kernelvalue;
						}
					}
				}
				output.at<G>(i, j) = (G)(w_st/w_ij);

			}
			else if (!strcmp(opt, "mirroring")) {
				w_st = 0;
				w_ij = 0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

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
						G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
						G_sigma_r= exp(-(pow(input.at<G>(i, j) - input.at<G>(tempa, tempb), 2)) / (2 * pow(sigma_r, 2)));
						kernelvalue = G_sigma_s * G_sigma_r;
						w_ij += kernelvalue;
						w_st += kernelvalue * (float)(input.at<G>(tempa, tempb));
					}

				}
				output.at<G>(i, j) = (G)(w_st / w_ij);

			}

			else if (!strcmp(opt, "adjustkernel")) {
				w_st = 0;
				w_ij = 0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
							G_sigma_r= exp(-(pow(input.at<G>(i, j) - input.at<G>(i + a, j + b), 2)) / (2 * pow(sigma_r, 2)));
							kernelvalue = G_sigma_s * G_sigma_r;
							w_ij += kernelvalue;
							w_st += kernelvalue * (float)(input.at<G>(i + a, j + b));
							
						}
					}
				}
				output.at<G>(i, j) = (G)(w_st / w_ij);

			}
		}
	}

	return output;
}
Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {
	Mat kernel_spatial;

	int tempa;
	int tempb;

	float kernelvalue;
	float G_sigma_r;
	float G_sigma_s;

	float w_ij[3] = { 0.0 };
	float w_st[3] = { 0.0 };

	int kernel_size = (2 * n + 1);
	kernel_spatial = Mat::zeros(kernel_size, kernel_size, CV_32F);

	int row = input.rows;
	int col = input.cols;

	for (int a = -n; a <= n; a++) {
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2))));
			kernel_spatial.at<float>(a + n, b + n) = value1;
		}
	}
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {
				for (int k = 0; k < 3; k++)
				{
					w_st[k] = 0;
					w_ij[k] = 0;
				}
				
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							//if the pixel is not a border pixel
							for (int c = 0; c < 3; c++)
							{
								G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
								G_sigma_r = exp(-(pow(input.at<C>(i, j)[c] - input.at<C>(i + a, j + b)[c], 2)) / (2 * pow(sigma_r, 2)));
								kernelvalue = G_sigma_s * G_sigma_r;
								w_ij[c] += kernelvalue;
								w_st[c] += kernelvalue * (float)(input.at<C>(i + a, j + b)[c]);
							}
							
						}
						else
						{
							//0인 값은 삭제
							for (int c = 0; c < 3; c++)
							{
								G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
								G_sigma_r = exp(-(pow(input.at<C>(i, j)[c], 2)) / (2 * pow(sigma_r, 2)));
								kernelvalue = G_sigma_s * G_sigma_r;
								w_ij[c] += kernelvalue;
							}
							
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(w_st[0] / w_ij[0]);
				output.at<C>(i, j)[1] = (G)(w_st[1] / w_ij[1]);
				output.at<C>(i, j)[2] = (G)(w_st[2] / w_ij[2]);

			}
			else if (!strcmp(opt, "mirroring")) {
				for (int k = 0; k < 3; k++)
				{
					w_st[k] = 0;
					w_ij[k] = 0;
				}

				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

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

						for (int c=0; c < 3; c++)
						{
							G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
							G_sigma_r = exp(-(pow(input.at<C>(i, j)[c] - input.at<C>(tempa, tempb)[c], 2)) / (2 * pow(sigma_r, 2)));
							kernelvalue = G_sigma_s * G_sigma_r;
							w_ij[c] += kernelvalue;
							w_st[c] += kernelvalue * (float)(input.at<C>(tempa, tempb)[c]);
						}
						
					}

				}
				output.at<C>(i, j)[0] = (G)(w_st[0] / w_ij[0]);
				output.at<C>(i, j)[1] = (G)(w_st[1] / w_ij[1]);
				output.at<C>(i, j)[2] = (G)(w_st[2] / w_ij[2]);

			}

			else if (!strcmp(opt, "adjustkernel")) {
				for (int k = 0; k < 3; k++)
				{
					w_st[k] = 0;
					w_ij[k] = 0;
				}
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							for (int c=0; c < 3; c++)
							{
								G_sigma_s = kernel_spatial.at<float>(a + n, b + n);
								G_sigma_r = exp(-(pow(input.at<C>(i, j)[c] - input.at<C>(i + a, j + b)[c], 2)) / (2 * pow(sigma_r, 2)));
								kernelvalue = G_sigma_s * G_sigma_r;
								w_ij[c] += kernelvalue;
								w_st[c] += kernelvalue * (float)(input.at<C>(i + a, j + b)[c]);
							}
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(w_st[0] / w_ij[0]);
				output.at<C>(i, j)[1] = (G)(w_st[1] / w_ij[1]);
				output.at<C>(i, j)[2] = (G)(w_st[2] / w_ij[2]);

			}



		}
	}

	return output;
}

