1. adaptivethreshold.cpp

- Purpose of this code
회색이미지에 대해 moving average를 이용한 adpative thresholding을 적용한 이미지를 반환해주는 코드이다.

1) adaptive_thres
- parameter:
	const Mat input : 원본 이미지
	int n : 커널 사이즈 2n+1에서의 n
	float bnumber : weight b
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈
	float temp : weight와 mean intensity를 곱한 값으로 T를 의미한다.
	Mat output : adpative thresholding을 적용한 이미지

2)main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수	
	Mat output : adpative thresholding을 적용한 이미지

2. kmeans.cpp
- Purpose of this code
컬러, 회색이미지에 대해 k-means clustering을 이용한 segmentation 이미지를 반환해주는 코드이다.

1) k_means_color
- parameter:
	const Mat input : 원본 이미지
	int clusterCount : k-means에서 사용할 cluster 개수
	int attempts : k-means에서 사용할 파라미터 attempts

-variables:
	Mat centers: k-means 에서 반환된 output array로 clustering된 값들이 저장됨
	Mat labels: k-means에서 반환된 bestLables
	Mat samples: k-means에 들어갈 input 값으로 (r,g,b) intensity가 저장됨
	Mat new_images: k-means clustering으로 segmentation된 이미지
2) k_means_gray
- parameter:
	const Mat input : 원본 이미지
	int clusterCount : k-means에서 사용할 cluster 개수
	int attempts : k-means에서 사용할 파라미터 attempts

-variables:
	Mat centers: k-means 에서 반환된 output array로 clustering된 값들이 저장됨
	Mat labels: k-means에서 반환된 bestLables
	Mat samples: k-means에 들어갈 input 값으로  intensity가 저장됨
	Mat new_images: k-means clustering으로 segmentation된 이미지

3) k_means_color_position
- parameter:
	const Mat input : 원본 이미지
	int clusterCount : k-means에서 사용할 cluster 개수
	int attempts : k-means에서 사용할 파라미터 attempts

-variables:
	Mat centers: k-means 에서 반환된 output array로 clustering된 값들이 저장됨
	Mat labels: k-means에서 반환된 bestLables
	Mat samples: k-means에 들어갈 input 값으로 (r, g ,b, x/σ, y/σ)가 저장됨
	Mat new_images: k-means clustering으로 segmentation된 이미지

4) k_means_gray_position
- parameter:
	const Mat input : 원본 이미지
	int clusterCount : k-means에서 사용할 cluster 개수
	int attempts : k-means에서 사용할 파라미터 attempts

-variables:
	Mat centers: k-means 에서 반환된 output array로 clustering된 값들이 저장됨
	Mat labels: k-means에서 반환된 bestLables
	Mat samples: k-means에 들어갈 input 값으로 (intensity, x/σ, y/σ)가 저장됨
	Mat new_images: k-means clustering으로 segmentation된 이미지
5) main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수	
	Mat k_means_color_output : 컬러이미지에 대해 k-means clustering으로 segmentation된 이미지
	Mat k_means_gray_output : 회색이미지에 대해 k-means clustering으로 segmentation된 이미지
	Mat k_means_color_postion_output : 컬러이미지에 대해 position 정보도 함께 사용해
					k-means clustering으로 segmentation된 이미지
	Mat k_means_gray_postion_output : 회색이미지에 대해 position 정보도 함께 사용해
					k-means clustering으로 segmentation된 이미지





