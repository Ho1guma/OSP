#include <opencv2/opencv.hpp>
#include <stdio.h>

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

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp);
Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt);
Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char *opt);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale

												// Add noise to original image
	Mat noise_Gray = Add_salt_pepper_Noise(input_gray, 0.1f, 0.1f);
	Mat noise_RGB = Add_salt_pepper_Noise(input, 0.1f, 0.1f);

	// Denoise, using median filter
	int window_radius = 2;
	Mat Denoised_Gray = Salt_pepper_noise_removal_Gray(noise_Gray, window_radius, "adjustkernel");
	Mat Denoised_RGB = Salt_pepper_noise_removal_RGB(noise_RGB, window_radius, "zero-padding");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Impulse Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (Grayscale)", noise_Gray);

	namedWindow("Impulse Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp)
{
	Mat output = input.clone();
	RNG rng;

	int amount1 = (int)(output.rows * output.cols * pp);
	int amount2 = (int)(output.rows * output.cols * ps);

	int x, y;

	// Grayscale image
	if (output.channels() == 1) {
		for (int counter = 0; counter < amount1; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 0;

		for (int counter = 0; counter < amount2; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 255;
	}
	// Color image	
	else if (output.channels() == 3) {
		for (int counter = 0; counter < amount1; ++counter) {
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 0;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 0;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 0;
		}

		for (int counter = 0; counter < amount2; ++counter) {
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 255;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 255;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 255;
		}
	}

	return output;
}


Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	int median = 0;

	// initialize median filter kernel
	Mat kernel = Mat::zeros(kernel_size * kernel_size, 1, input.type());
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int idx = 0;
			if (!strcmp(opt, "zero-padding")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							//if the pixel is not a border pixel
							kernel.at<G>(idx++, 0) = (int)(input.at<G>(i + x, j + y));
						}
						else //zero padding
							kernel.at<G>(idx++, 0) = 0;
					}
				}
				median = int(idx / 2);
			}

			else if (!strcmp(opt, "mirroring")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						if (i + x > row - 1) {  //mirroring for the border pixels
							tempa = i - x;
						}
						else if (i + x < 0) {
							tempa = -(i + x);
						}
						else {
							tempa = i + x;
						}
						if (j + y > col - 1) {
							tempb = j - y;
						}
						else if (j + y < 0) {
							tempb = -(j + y);
						}
						else {
							tempb = j + y;
						}
						kernel.at<G>(idx++, 0) = (int)(input.at<G>(tempa, tempb));
					}
				}
				median = int(idx / 2);
			}

			else if (!strcmp(opt, "adjustkernel")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							kernel.at<G>(idx++, 0) = (int)(input.at<G>(i + x, j + y));
						}
					}
				}
				
				median = int(idx / 2);
				median = median + (kernel_size * kernel_size - idx);
			}
			sort(kernel, kernel, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
			output.at<G>(i, j) = kernel.at<G>(median,0);
		}
	}

	return output;
}

Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int median_r=0;// index of median value
	int median_g=0;
	int median_b=0;
	int idx_r = 0;
	int idx_g = 0;
	int idx_b = 0;
	int channel = input.channels();
	int tempa;
	int tempb;

	// initialize ( (TypeX with 3 channel) - (TypeX with 1 channel) = 16 )
	// ex) CV_8UC3 - CV_8U = 16
	Mat kernel = Mat::zeros(kernel_size * kernel_size, channel, input.type() - 16);

	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			median_r = median_g = median_b =idx_r =idx_g =idx_b = 0;
			if (!strcmp(opt, "zero-padding")) {
				
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							//if the pixel is not a border pixel
							kernel.at<G>(idx_r++, 0) = (int)(input.at<C>(i + x, j + y)[0]);
							kernel.at<G>(idx_g++, 1) = (int)(input.at<C>(i + x, j + y)[1]);
							kernel.at<G>(idx_b++, 2) = (int)(input.at<C>(i + x, j + y)[2]);
						}
						else {//zero padding
							kernel.at<G>(idx_r++, 0) = 0;
							kernel.at<G>(idx_g++, 1) = 0;
							kernel.at<G>(idx_b++, 2) = 0;
						}

					}
				}
				median_r = int(idx_r / 2);
				median_g = int(idx_g / 2);
				median_b = int(idx_b / 2);
			}

			else if (!strcmp(opt, "mirroring")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						if (i + x > row - 1) {  //mirroring for the border pixels
							tempa = i - x;
						}
						else if (i + x < 0) {
							tempa = -(i + x);
						}
						else {
							tempa = i + x;
						}
						if (j + y > col - 1) {
							tempb = j - y;
						}
						else if (j + y < 0) {
							tempb = -(j + y);
						}
						else {
							tempb = j + y;
						}
						kernel.at<G>(idx_r++, 0) = (int)(input.at<C>(tempa, tempb)[0]);
						kernel.at<G>(idx_g++, 1) = (int)(input.at<C>(tempa, tempb)[1]);
						kernel.at<G>(idx_b++, 2) = (int)(input.at<C>(tempa, tempb)[2]);
					}
				}
				median_r = int(idx_r / 2);
				median_g = int(idx_g / 2);
				median_b = int(idx_b / 2);
			}
			else if (!strcmp(opt, "adjustkernel")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							kernel.at<G>(idx_r++, 0) = (int)(input.at<C>(i + x, j + y)[0]);
							kernel.at<G>(idx_g++, 0) = (int)(input.at<C>(i + x, j + y)[1]);
							kernel.at<G>(idx_b++, 0) = (int)(input.at<C>(i + x, j + y)[2]);
						}
					}
				}
				
				median_r = int(idx_r / 2+(kernel_size * kernel_size - idx_r));
				median_g = int(idx_g / 2 + (kernel_size * kernel_size - idx_g));
				median_b = int(idx_b / 2 + (kernel_size * kernel_size - idx_b));
			}

			// Sort the kernels in ascending order
			sort(kernel, kernel, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);

			output.at<C>(i, j)[0] = kernel.at<G>(median_r, 0);
			output.at<C>(i, j)[1] = kernel.at<G>(median_g, 1);
			output.at<C>(i, j)[2] = kernel.at<G>(median_b, 2);
			
		}
	}
	return output;
}