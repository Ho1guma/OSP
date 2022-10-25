#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

template <typename T>
Mat myrotate(const Mat input, float angle, const char* opt);

int main()
{
	Mat input, rotated;
	
	// Read each image
	input = imread("lena.jpg");

	// Check for invalid input
	if (!input.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	
	// original image
	namedWindow("image");
	imshow("image", input);

	rotated = myrotate<Vec3b>(input, 45, "bilinear");

	// rotated image
	namedWindow("rotated");
	imshow("rotated", rotated);

	waitKey(0);

	return 0;
}

template <typename T>
Mat myrotate(const Mat input, float angle, const char* opt) {
	int row = input.rows;
	int col = input.cols;

	float radian = angle * CV_PI / 180;

	float sq_row = ceil(row * sin(radian) + col * cos(radian));
	float sq_col = ceil(col * sin(radian) + row * cos(radian));

	Mat output = Mat::zeros(sq_row, sq_col, input.type());

	for (int i = 0; i < sq_row; i++) {
		for (int j = 0; j < sq_col; j++) {
			//x->j y->i

			float x = (j - sq_col / 2) * cos(radian) - (i - sq_row / 2) * sin(radian) + col / 2;//cos - sin
			float y = (j - sq_col / 2) * sin(radian) + (i - sq_row / 2) * cos(radian) + row / 2;//sin cos
			//x' y'

			//compute f(x') f(y')
			if ((y >= 0) && (y <= (row - 1)) && (x >= 0) && (x <= (col - 1))) {
				if (!strcmp(opt, "nearest")) {
					float y_int, x_int;
					y_int = floor(y+0.5); //실수값인 x,y를 반올림하여 가장 가까운 정수값을 구해준다.
					x_int = floor(x+0.5);
					//<Vec3f>에서 에러 발생... <Vec3b>로 수정
					output.at<Vec3b>(i, j) = input.at<Vec3b>(y_int, x_int);

				}
				else if (!strcmp(opt, "bilinear")) {
					float y1 = floor(y); //가장 가까운 정수 좌표 4개를 구해준다.
					float y2 = ceil(y);
					float x1 = floor(x);
					float x2 = ceil(x);

					float mu = y - y1; //뮤, 람다를 구해준다. 
					float lamda = x - x1;

					//공식에 맞추어 계산
					output.at<Vec3b>(i, j) = lamda*(mu* input.at<Vec3b>(y2, x2)+(1-mu)* input.at<Vec3b>(y1,x2))+
						(1-lamda)*(mu* input.at<Vec3b>(y2,x1)+(1-mu)* input.at<Vec3b>(y1,x1));
				}
			}
		}
	}

	return output;
}