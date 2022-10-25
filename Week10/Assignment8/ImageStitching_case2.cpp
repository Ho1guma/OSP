﻿#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#define RATIO_THR 0.4

using namespace cv;
using namespace std;

double euclidDistance(Mat& vec1, Mat& vec2);
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, bool second);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold);

template <typename T>
Mat cal_affine(vector<Point2f> ptl, vector<Point2f> ptr, int number_of_points);

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha);


int main() {
	Mat I1, I2;

	// Read each image
	I1 = imread("input1.jpg");
	I2 = imread("input2.jpg");
	Mat I1_gray, I2_gray;

	// Check for invalid input
	if (!I1.data || !I2.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	resize(I1, I1, Size(I1.cols / 2, I1.rows / 2));
	resize(I2, I2, Size(I2.cols / 2, I2.rows / 2));

	cvtColor(I1, I1_gray, CV_RGB2GRAY);
	cvtColor(I2, I2_gray, CV_RGB2GRAY);

	I1.convertTo(I1, CV_32FC3, 1.0 / 255);
	I2.convertTo(I2, CV_32FC3, 1.0 / 255);

	FeatureDetector* detector = new SiftFeatureDetector(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);
	DescriptorExtractor* extractor = new SiftDescriptorExtractor();

	// Compute keypoints and descriptor from the source image in advance
	vector<KeyPoint> keypoints1;
	Mat descriptors1;

	detector->detect(I1_gray, keypoints1);
	extractor->compute(I1_gray, keypoints1, descriptors1);
	printf("input1 : %d keypoints are found.\n", (int)keypoints1.size());

	vector<KeyPoint> keypoints2;
	Mat descriptors2;

	// Detect keypoints
	detector->detect(I2_gray, keypoints2);
	extractor->compute(I2_gray, keypoints2, descriptors2);
	printf("input2 : %zd keypoints are found.\n", keypoints2.size());

	// Find nearest neighbor pairs
	vector<Point2f> srcPoints;
	vector<Point2f> dstPoints;

	bool crossCheck = true;
	bool ratio_threshold = true;
	findPairs(keypoints2, descriptors2, keypoints1, descriptors1, srcPoints, dstPoints, crossCheck, ratio_threshold);
	printf("%zd keypoints are matched.\n", srcPoints.size());

	// corresponding pixels	(기존에 주어졌던 픽셀값들)
	//int ptl_x[28] = { 509, 558, 605, 649, 680, 689, 705, 730, 734, 768, 795, 802, 818, 837, 877, 889, 894, 902, 917, 924, 930, 948, 964, 969, 980, 988, 994, 998 };
	//int ptl_y[28] = { 528, 597, 581, 520, 526, 581, 587, 496, 506, 500, 342, 558, 499, 642, 474, 456, 451, 475, 530, 381, 472, 475, 426, 539, 329, 341, 492, 511 };	
	//int ptr_x[28] = { 45, 89, 142, 194, 226, 230, 246, 279, 281, 314, 352, 345, 365, 372, 421, 434, 439, 446, 456, 472, 471, 488, 506, 503, 527, 532, 528, 531 };
	//int ptr_y[28] = { 488, 561, 544, 482, 490, 546, 552, 462, 471, 467, 313, 526, 468, 607, 445, 429, 424, 447, 500, 358, 446, 449, 403, 510, 312, 324, 466, 484 };
	const float I1_row = I1.rows;
	const float I1_col = I1.cols;
	const float I2_row = I2.rows;
	const float I2_col = I2.cols;


	//S = log(1-P)/log(1-(1-e^k)) k=3
	int k[3];
	int S = 10;
	double sigma = 3;
	int cnt = srcPoints.size();
	double diff;
	int score_max = 0, score = 0;
	Mat T;
	vector<Point2f> src, dst,inliers_src, inliers_dst, best_src, best_dst;
	src.clear(); dst.clear(); inliers_src.clear(); inliers_dst.clear(); best_src.clear(); best_dst.clear();
	
	
	srand(time(0));
	for (int i = 0; i < S; i++) {
		// Randomly sample k data
		for (;;)
		{
			k[0] = rand() % cnt;
			k[1] = rand() % cnt;
			k[2] = rand() % cnt;
			if (k[0] != k[1] && k[1] != k[2] && k[0] != k[2]) break;
		}
		
		for (int j = 0; j < 3; j++)
		{
			src.push_back(srcPoints.at(k[j])); 
			dst.push_back(dstPoints.at(k[j]));
		}
	
		// Estimate the affine transformation T by solving Mx=b.
		T = cal_affine<float>(src, dst, 3);
		diff = 0.0;  score = 0; 

		//Score by computing the number of inliers satisfying |Tp − p|^2 < sigma^2 from all matches.
		for (int j = 0; j < srcPoints.size(); j++) {
			float x = T.at<float>(0) * srcPoints.at(i).x + 
				T.at<float>(1) * srcPoints.at(i).y + T.at<float>(2); // TP.x
			float y = T.at<float>(3) * srcPoints.at(i).x 
				+ T.at<float>(4) * srcPoints.at(i).y + T.at<float>(5); // TP.y
			diff = pow(abs(x - dstPoints.at(i).x), 2)+ pow(abs(y - dstPoints.at(i).y), 2);
			if (diff < pow(sigma, 2)) {
				score++; 
				inliers_src.push_back(srcPoints.at(j)); 
				inliers_dst.push_back(dstPoints.at(j)); 
			}
		}
		//Select the best affine transformation
		if (score > score_max) {
			score_max = score;
			best_src = inliers_src;
			best_dst = inliers_dst;
		}
	}
	//Re-estimate the affine transformation by solving Mx=b with Tb’s inliers.
	Mat A12 = cal_affine<float>(best_dst, best_src, (int)best_dst.size());
	Mat A21 = cal_affine<float>(best_src, best_dst, (int)best_src.size());
	

	Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));
	Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * I2_row + A21.at<float>(5));
	Point2f p3(A21.at<float>(0) * I2_col + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * I2_row + A21.at<float>(5));
	Point2f p4(A21.at<float>(0) * I2_col + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * 0 + A21.at<float>(5));


	// compute boundary for merged image(I_f)
	// bound_u <= 0
	// bound_b >= I1_row-1
	// bound_l <= 0
	// bound_b >= I1_col-1
	int bound_u = (int)round(min(0.0f, min(p1.y, p4.y)));
	int bound_b = (int)round(max(I1_row - 1, max(p2.y, p3.y)));
	int bound_l = (int)round(min(0.0f, min(p1.x, p2.x)));
	int bound_r = (int)round(max(I1_col - 1, max(p3.x, p4.x)));

	// initialize merged image
	Mat I_f(bound_b - bound_u + 1, bound_r - bound_l + 1, CV_32FC3, Scalar(0));

	// inverse warping with bilinear interplolation
	for (int i = bound_u; i <= bound_b; i++) {
		for (int j = bound_l; j <= bound_r; j++) {
			float x = A12.at<float>(0) * j + A12.at<float>(1) * i + A12.at<float>(2) - bound_l;
			float y = A12.at<float>(3) * j + A12.at<float>(4) * i + A12.at<float>(5) - bound_u;

			float y1 = floor(y);
			float y2 = ceil(y);
			float x1 = floor(x);
			float x2 = ceil(x);

			float mu = y - y1;
			float lambda = x - x1;

			if (x1 >= 0 && x2 < I2_col && y1 >= 0 && y2 < I2_row)
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = lambda * (mu * I2.at<Vec3f>(y2, x2) + (1 - mu) * I2.at<Vec3f>(y1, x2)) +
														  (1 - lambda) *(mu * I2.at<Vec3f>(y2, x1) + (1 - mu) * I2.at<Vec3f>(y1, x1));
		}
	}
	// image stitching with blend
	blend_stitching(I1, I2, I_f, bound_l, bound_u, 0.5);

	namedWindow("Left Image");
	imshow("Left Image", I1);

	namedWindow("Right Image");
	imshow("Right Image", I2);

	namedWindow("result");
	imshow("result", I_f);

	I_f.convertTo(I_f, CV_8UC3, 255.0);
	imwrite("result.png", I_f);

	waitKey(0);

	return 0;
}

template <typename T>
Mat cal_affine(vector<Point2f> ptl, vector<Point2f> ptr, int number_of_points) {

	Mat M(2 * number_of_points, 6, CV_32F, Scalar(0));
	Mat b(2 * number_of_points, 1, CV_32F);

	Mat M_trans, temp, affineM;

	// initialize matrix
	for (int i = 0; i < number_of_points; i++) {
		M.at<T>(2 * i, 0) = ptl[i].x;		M.at<T>(2 * i, 1) = ptl[i].y;		M.at<T>(2 * i, 2) = 1;
		M.at<T>(2 * i + 1, 3) = ptl[i].x;		M.at<T>(2 * i + 1, 4) = ptl[i].y;		M.at<T>(2 * i + 1, 5) = 1;
		b.at<T>(2 * i) = ptr[i].x;		b.at<T>(2 * i + 1) = ptr[i].y;
	}

	// (M^T * M)^(−1) * M^T * b ( * : Matrix multiplication)
	transpose(M, M_trans);
	invert(M_trans * M, temp);
	affineM = temp * M_trans * b;

	return affineM;
}

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int bound_l, int bound_u, float alpha) {

	int col = I_f.cols;
	int row = I_f.rows;

	// I2 is already in I_f by inverse warping
	for (int i = 0; i < I1.rows; i++) {
		for (int j = 0; j < I1.cols; j++) {
			bool cond_I2 = I_f.at<Vec3f>(i - bound_u, j - bound_l) != Vec3f(0, 0, 0) ? true : false;

			if (cond_I2)
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = alpha * I1.at<Vec3f>(i, j) + (1 - alpha) * I_f.at<Vec3f>(i - bound_u, j - bound_l);
			else
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = I1.at<Vec3f>(i, j);

		}
	}
}

/**
* Calculate euclid distance
*/
double euclidDistance(Mat& vec1, Mat& vec2) { //Compute on row vector
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < dim; i++) {
		sum += (vec1.at<float>(0, i) - vec2.at<float>(0, i)) * (vec1.at<float>(0, i) - vec2.at<float>(0, i));
	}

	return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, bool second) {
	int neighbor = -1;
	int neighbor2 = -1;
	double minDist = 1e6;
	double minDist2 = 1e6;
	double dist;
	double dist2;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor
		dist = euclidDistance(vec, v);
		if (dist < minDist)
		{
			minDist = dist;
			neighbor = i;
		}

	}

	if (second == true)
	{
		for (int j = 0; j < descriptors.rows; j++) {
			if (j == neighbor)
				continue;
			Mat u = descriptors.row(j);
			dist2 = euclidDistance(vec, u);
			if (dist2 < minDist2)
			{
				minDist2 = dist2;
				neighbor2 = j;
			}

		}
		return neighbor2;
	}

	return neighbor;
}

/**
* Find pairs of points with the smallest distace between them
*/
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {
		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2, false);
		int nn2 = nearestNeighbor(desc1, keypoints2, descriptors2, true);
		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {
			Mat f_i = desc1;
			Mat g_1 = descriptors2.row(nn); //g_j
			Mat g_2 = descriptors2.row(nn2);
			double k1 = euclidDistance(f_i, g_1);
			double k2 = euclidDistance(f_i, g_2);

			if ((k1 / k2) > RATIO_THR)
				continue;
		}

		// Refine matching points using cross-checking
		if (crossCheck) {
			Mat f_i = desc1;
			Mat g_k = descriptors2.row(nn);
			int L = nearestNeighbor(g_k, keypoints1, descriptors1, false);
			if (L != i)
				continue;
		}
		KeyPoint pt2 = keypoints2[nn];
		srcPoints.push_back(pt1.pt);
		dstPoints.push_back(pt2.pt);
	}
}