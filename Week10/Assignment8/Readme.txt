1. ImageStitching_case1

- Purpose of this code
이미지 2개에 대해 SIFT descriptor를 사용해 매칭된 특징점을 stitching한 이미지를 window창에 보여주는 코드이다.
(cross-checking과 ratio-based thresholding 기법 사용)

- How to run this code
이미지 2개 (input1.jpg, input2.jpg)와 같은 경로에 ImageStitching_case1.cpp를 넣고 코드를 실행시킨다.

- How to define default parameters

1) euclidDistance
- parameter:
	Mat& vec1 : 유클리디안 거리를 구할 벡터1 
	Mat& vec2 : 유클리디안 거리를 구할 백터2

2) nearestNeighbor
- parameter:
	Mat& vec : 가장 가까운 벡터를 구할 기준이 되는 벡터
	vector<KeyPoint>& keypoints : keypoints
	Mat& descriptors : 가장 가까운 벡터가 될 후보 벡터들
	bool second : 두번째로 가까운 벡터를 구할지 유/무
	

3) findPairs
- parameter:
	vector<KeyPoint>& keypoints1 :  image1에서 추출된 keypoints
	Mat& descriptors1 : keypoints1에 대한 descriptors
	vector<KeyPoint>& keypoints2 : image2에서 추출된 keypoints
	Mat& descriptors2 : keypoint2에 대한 descriptors
	vector<Point2f>& srcPoints : 일치하는 keypoints를 push_back하기 위한 포인터
	vector<Point2f>& dstPoints : 일치하는 keypoints를 push_back하기 위한 포인터
	bool crossCheck : cross-checking 옵션의 유/무
	bool ratio_threshold : ratio_thesholding 옵션의 유/무

4) cal_affine
-parameter:
	int ptl_x[] : 이미지 left의 x 값
	int ptl_y[] : 이미지 left의 y 값
	int ptr_x[] : 이미지 right의 x 값
	int ptr_y[] : 이미지 right의 y 값
	int number_of_points : 총 점의 개수

5) blend_stitching
	const Mat I1 : input1의 이미지
	const Mat I2 : input2의 이미지
	Mat &I_f : initialized merged image
	int diff_x : bound_l 값
	int diff_y : bound_u 값
	float alpha : 알파값=0.5

- How to adjust parameters (if any)
2)nearestNeighbor
bool second  : 첫번째로 가까운 점의 인덱스를 구하고 싶다면 false, 두번째로 가까운 점의 인덱스를 구하고 싶다면 true로 설정
3) findPairs
bool crossCheck : cross-checking 옵션을 주고 싶다면 true, 아니라면 false로 설정
bool ratio_threshold : ratio_thesholding 옵션을 주고 싶다면 true, 아니라면 false로 설정

2개의 이미지에 매칭되는 픽셀값들은 적어도 3개이상이 필요하다. 
또한 affine된 이미지를 생성하는 과정에서 bilinear interpolation으로 구현되었으며,
이미지를 합치는 blend_stitching에서의 alpha값은 0.5가 디폴트로 설정되어있다.


1. ImageStitching_case2

- Purpose of this code
이미지 2개에 대해 SIFT descriptor를 사용해 매칭된 특징점으로  RANSAC을 이용해 affine transform을 구하고
이를  stitching한 이미지를 window창에 보여주는 코드이다.
(cross-checking과 ratio-based thresholding 기법 사용)

- How to run this code
이미지 2개 (input1.jpg, input2.jpg)와 같은 경로에 ImageStitching_case2.cpp를 넣고 코드를 실행시킨다.

- How to define default parameters

1) euclidDistance
- parameter:
	Mat& vec1 : 유클리디안 거리를 구할 벡터1 
	Mat& vec2 : 유클리디안 거리를 구할 백터2

2) nearestNeighbor
- parameter:
	Mat& vec : 가장 가까운 벡터를 구할 기준이 되는 벡터
	vector<KeyPoint>& keypoints : keypoints
	Mat& descriptors : 가장 가까운 벡터가 될 후보 벡터들
	bool second : 두번째로 가까운 벡터를 구할지 유/무
	

3) findPairs
- parameter:
	vector<KeyPoint>& keypoints1 :  image1에서 추출된 keypoints
	Mat& descriptors1 : keypoints1에 대한 descriptors
	vector<KeyPoint>& keypoints2 : image2에서 추출된 keypoints
	Mat& descriptors2 : keypoint2에 대한 descriptors
	vector<Point2f>& srcPoints : 일치하는 keypoints를 push_back하기 위한 포인터
	vector<Point2f>& dstPoints : 일치하는 keypoints를 push_back하기 위한 포인터
	bool crossCheck : cross-checking 옵션의 유/무
	bool ratio_threshold : ratio_thesholding 옵션의 유/무

4) cal_affine
-parameter:
	int ptl_x[] : 이미지 left의 x 값
	int ptl_y[] : 이미지 left의 y 값
	int ptr_x[] : 이미지 right의 x 값
	int ptr_y[] : 이미지 right의 y 값
	int number_of_points : 총 점의 개수

5) blend_stitching
	const Mat I1 : input1의 이미지
	const Mat I2 : input2의 이미지
	Mat &I_f : initialized merged image
	int diff_x : bound_l 값
	int diff_y : bound_u 값
	float alpha : 알파값=0.5

- How to adjust parameters (if any)
2)nearestNeighbor
bool second  : 첫번째로 가까운 점의 인덱스를 구하고 싶다면 false, 두번째로 가까운 점의 인덱스를 구하고 싶다면 true로 설정
3) findPairs
bool crossCheck : cross-checking 옵션을 주고 싶다면 true, 아니라면 false로 설정
bool ratio_threshold : ratio_thesholding 옵션을 주고 싶다면 true, 아니라면 false로 설정

RANSAC
k : 랜덤으로 뽑을 특징점의 개수
s : 반복 횟수
sigma : |Tp − p|^2 < sigma^2 에서 threshold 역할을 하게 된다.


3. 






