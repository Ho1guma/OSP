#include <iostream>
#include <opencv2/opencv.hpp>

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

Mat k_means_color(Mat input, int clusterCount, int attempts);
Mat k_means_gray(Mat input, int clusterCount, int attempts);
Mat k_means_color_position(Mat input, int clusterCount, int attempts);
Mat k_means_gray_position(Mat input, int clusterCount, int attempts);

// Note that this code is for the case when an input data is a color value.
int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;
	cvtColor(input, input_gray, CV_RGB2GRAY);

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);

	int clusterCount = 10;
	int attempts = 5;

	Mat k_means_color_output(input.size(), input.type());
	k_means_color_output = k_means_color(input,clusterCount, attempts);

	Mat k_means_gray_output(input.size(), input.type());
	k_means_gray_output = k_means_gray(input_gray, clusterCount, attempts);

	Mat k_means_color_position_output(input.size(), input.type());
	k_means_color_position_output = k_means_color_position(input, clusterCount, attempts);

	Mat k_means_gray_position_output(input.size(), input.type());
	k_means_gray_position_output = k_means_gray_position(input_gray, clusterCount, attempts);

	imshow("clustered image(color)", k_means_color_output);
	imshow("clustered image(gray)", k_means_gray_output);
	imshow("clustered image(color+position)", k_means_color_position_output);
	imshow("clustered image(gray+position)", k_means_gray_position_output);

	waitKey(0);

	return 0;
}

Mat k_means_color(Mat input, int clusterCount,int attempts)
{
	Mat centers;
	Mat labels;
	Mat samples(input.rows * input.cols, 3, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x * input.rows, z) = input.at<Vec3b>(y, x)[z];
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001)
		, attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * input.rows, 0);
			//Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			new_image.at<C>(y, x)[0] = (G)(int)(centers.at<float>(cluster_idx, 0));
			new_image.at<C>(y, x)[1] = (G)(int)(centers.at<float>(cluster_idx, 1));
			new_image.at<C>(y, x)[2] = (G)(int)(centers.at<float>(cluster_idx, 2));
		}

	return new_image;
}
Mat k_means_gray(Mat input, int clusterCount, int attempts)
{
	Mat centers;
	Mat labels;
	Mat samples(input.rows * input.cols, 1, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
			samples.at<float>(y + x * input.rows, 0) = input.at<G>(y, x);

	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001)
		, attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * input.rows, 0);
			new_image.at<G>(y, x)= (G)(int)(centers.at<float>(cluster_idx, 0));
		}

	return new_image;
}
Mat k_means_color_position(Mat input, int clusterCount, int attempts)
{
	Mat centers;
	Mat labels;
	Mat samples(input.rows * input.cols, 5, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			samples.at<float>(y + x * input.rows, 0) = input.at<Vec3b>(y, x)[0]; //r
			samples.at<float>(y + x * input.rows, 1) = input.at<Vec3b>(y, x)[1]; //g
			samples.at<float>(y + x * input.rows, 2) = input.at<Vec3b>(y, x)[2]; //b
			samples.at<float>(y + x * input.rows, 3) = (x / input.cols); //x , normalizing in [0~1]
			samples.at<float>(y + x * input.rows, 4) = (y / input.rows); //y , normalizing in [0~1]

		}
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001)
		, attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * input.rows, 0);
			//Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			new_image.at<C>(y, x)[0] = (G)(int)(centers.at<float>(cluster_idx, 0));
			new_image.at<C>(y, x)[1] = (G)(int)(centers.at<float>(cluster_idx, 1));
			new_image.at<C>(y, x)[2] = (G)(int)(centers.at<float>(cluster_idx, 2));
		}

	return new_image;
}
Mat k_means_gray_position(Mat input, int clusterCount, int attempts)
{
	Mat centers;
	Mat labels;
	Mat samples(input.rows * input.cols, 3, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			samples.at<float>(y + x * input.rows, 0) = input.at<G>(y, x);
			samples.at<float>(y + x * input.rows, 1) = (x / input.cols); //x , normalizing in [0~1]
			samples.at<float>(y + x * input.rows, 2) = (y / input.rows); //y , normalizing in [0~1]
		}

	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001)
		, attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * input.rows, 0);
			new_image.at<G>(y, x) = (G)(int)(centers.at<float>(cluster_idx, 0));
		}

	return new_image;
}

