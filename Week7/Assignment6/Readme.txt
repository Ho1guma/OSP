1. LoG.cpp

- Purpose of this code
회색이미지에 대해 Laplaciain of Gaussian을 적용해 이미지에 대한 Edge를 보여주는 코드이다.

1) Gaussianfilter
- parameter:
	const Mat input : 원본 이미지
	int n : 커널 사이즈 2n+1에서의 n
	double sigma_t : 가우시안 필터에서 사용할 시그마t
	double sigma_s : 가우시안 필터에서 사용할 시그마s
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈
	Mat output : 가우시안 필터를 적용한 이미지
	Mat input_mirror : input에 대해 mirroring을 적용한 이미지

2) Laplacianfilter
- parameter:
	const Mat input : 원본 이미지
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈
	Mat output : 라플라시안 필터를 적용한 이미지
	Mat input_mirror : input에 대해 mirroring을 적용한 이미지

3) Mirroring
- parameter:
	const Mat input : 원본 이미지
	int n : 커널 사이즈 2n+1에서의 n
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	int row2 : mirroring 이미지의 행 개수
	int col2 : mirroring 이미지의 열 개수
	Mat input2 : mirroring를 적용한 이미지

4) get_Gaussian_Kernel
-parameter
	int n : 커널 사이즈 2n+1에서의 n
	double sigma_t : 가우시안 필터에서 사용할 시그마t
	double sigma_s : 가우시안 필터에서 사용할 시그마s
	bool normalized : normalization 유/무
-variables:
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈
	
5) get_Laplacian_Kernel
-variables:
	Mat kernel : 커널이 들어갈 변수
6) main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수
	int window_radius : 커널 사이즈 2n+1에서의 n
	double sigma_t : 가우시안 필터에서 사용할 시그마t
	double sigma_s : 가우시안 필터에서 사용할 시그마s
	Mat output : adpative thresholding을 적용한 이미지
	Mat h_f : 가우시안 필터를 적용한 이미지
	Mat Laplacian : h_f에 라플라시안 필터를 적용한 이미지

2. Canny.cpp
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수
	Mat output : canny를 적용한 이미지


3. Harris_corner.cpp
- Purpose of this code
이미지에 대해 Harris Corner Detector를 이용한 Corner를 circle로 표시한 이미지를 반환해주는 코드이다.

1) NonMaximum_Suppression
-parameters:
	const Mat input : 원본 이미지
	Mat corner_mat : corner인 pixel들이 표시된 변수
	int radius : window 사이즈
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat input_mirror : input에 대해 mirroring을 적용한 이미지

2) Mirroring
- parameter:
	const Mat input : 원본 이미지
	int n : 커널 사이즈 2n+1에서의 n
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	int row2 : mirroring 이미지의 행 개수
	int col2 : mirroring 이미지의 열 개수
	Mat input2 : mirroring를 적용한 이미지
3) main
	Mat input : 원본 이미지가 저장된 변수
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat corner_mat : corner인 pixel들이 표시된 변수
	bool NonMaxSupp : non-maximum suppression의 적용 유/무
	bool Subpixel : subpixel refinement의 적용 유/무
	int blockSize : cornerHarris에서 사용할 파라미터 blocksize
	int apertureSize : cornerHarris에서 사용할 파라미터 apertureSize 
	double k : cornerHarris에서 사용할 파라미터 k







