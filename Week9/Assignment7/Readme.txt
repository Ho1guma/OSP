1. SIFT.cpp

- Purpose of this code
이미지 2개에 대해 SIFT descriptor를 사용해 매칭된 특징점을 연결한 이미지를 window창에 보여주는 코드이다.
cross-checking과 ratio-based thresholding 기법의 사용 유/무를 조절해 사용할 수 있다.

- How to run this code
이미지 2개 (input1.jpg, input2.jpg)와 같은 경로에 SIFT.cpp를 넣고 코드를 실행시킨다.

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

- How to adjust parameters (if any)
2)nearestNeighbor
bool second  : 첫번째로 가까운 점의 인덱스를 구하고 싶다면 false, 두번째로 가까운 점의 인덱스를 구하고 싶다면 true로 설정
3) findPairs
bool crossCheck : cross-checking 옵션을 주고 싶다면 true, 아니라면 false로 설정
bool ratio_threshold : ratio_thesholding 옵션을 주고 싶다면 true, 아니라면 false로 설정

	






