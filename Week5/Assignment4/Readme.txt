1. salt_and_pepper.cpp

- Purpose of this code
컬러, 회색이미지에 대해 salt and pepper 노이즈를 추가한 이미지, median filter를 적용하여
이를 제거한 이미지를  window 창에 display해주는 코드이다.

1) Add_salt_pepper_Noise
- parameter:
	const Mat input : 원본 이미지
	float ps : salt noise가 발생할 확률
	float pp : pepper nosie가 발생할 확률
-variables:
	Mat output : 노이즈가 추가왼 이미지
	int amount1 : pepper 노이즈의 개수
	int amount2: salt 노이즈의 개수
	int x : 픽셀 x값
	int y : 픽셀 y값

2) Salt_pepper_noise_removal_Gray
-parameter:
	const Mat input : 원본 이미지 
	int n : 커널 사이즈 2n+1에서의 n
	const char* opt : median 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int median : 중간값이 저장될 변수
	Mat output : 커널을 적용해 나온 노이즈 제거된 이미지
	int idx : 커널에 저장된 변수 개수

3) Salt_pepper_noise_removal_RGB
-parameter:
	const Mat input : 원본 이미지 
	int n : 커널 사이즈 2n+1에서의 n
	const char* opt : median 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int median_r : red channel의 중간값이 저장될 변수
	int median_g : green channel의 중간값이 저장될 변수
	int median_b : blue channel의 중간값이 저장될 변수
	Mat output : 커널을 적용해 나온 노이즈 제거된 이미지
	int idx_r : red channel의 커널에 저장된 변수 개수
	int idx_g : green channel의 커널에 저장된 변수 개수
	int idx_b  : blue channel의 커널에 저장된 변수 개수

4) main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수	
	Mat noise_Gray : 회색이미지에 salt and pepper 노이즈가 적용된 이미지가 저장된 변수
 	Mat noise_RGB : 컬러이미지에 salt and pepper 노이즈가 적용된 이미지가 저장된 변수
	Mat Denoised_Gray : median 필터 적용 회색 이미지가 저장된 변수
	Mat Denoised_RGB : median 필터 적용 컬러 이미지가 저장된 변수


2. Gaussian.cpp
컬러, 회색이미지에 대해 가우시안 노이즈를 추가한 이미지, 가우시안 filter를 적용하여
이를 제거한 이미지를  window 창에 display해주는 코드이다.

1)Add_Gaussian_noise
-parameter:
	const Mat input : 노이즈를 추가할 원본이미지
	double mean : 가우시안 필터에서 사용할 평균값 
	double sigma : 가우시안 필터에서 사용할 시그마값
-variables:
	Mat NoiseArr: 노이즈 추가한 이미지

2)Gaussianfilter_Gray
-parameter:
	const Mat input:노이즈를 제거할 이미지
	int n : 커널사이즈
	double sigma_t : 가우시안 필터에서 사용할 시그마t값
	double sigma_s : 가우시안 필터에서 사용할 시그마s값
	const char *opt : 가우시안 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 전체 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	float denom: 필터값 를 유도할 때 사용할 분모값
	Mat output : temp에 대해 W_s를 적용해 나온 최종 커널 적용값

3)Gaussianfilter_RGB
-parameter:
	const Mat input:노이즈를 제거할 이미지
	int n : 커널사이즈
	double sigma_t : 가우시안 필터에서 사용할 시그마t값
	double sigma_s : 가우시안 필터에서 사용할 시그마s값
	const char *opt : 가우시안 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel : 전체 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	float denom: 필터값 를 유도할 때 사용할 분모값
	Mat output : temp에 대해 W_s를 적용해 나온 최종 커널 적용값

4) main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수	
	Mat noise_Gray : 회색이미지에 가우시안 노이즈가 적용된 이미지가 저장된 변수
 	Mat noise_RGB : 컬러이미지에 가우시안 노이즈가 적용된 이미지가 저장된 변수
	Mat Denoised_Gray : 가우시안 필터 적용 회색 이미지가 저장된 변수
	Mat Denoised_RGB : 가우시안 필터 적용 컬러 이미지가 저장된 변수

3 GaussianNoise_bilateralFiltering.cpp
컬러, 회색이미지에 대해 가우시안 노이즈를 추가한 이미지, bilateral filter를 적용하여
이를 제거한 이미지를  window 창에 display해주는 코드이다.

1)Add_Gaussian_noise
-parameter:
	const Mat input : 노이즈를 추가할 원본이미지
	double mean : 가우시안 필터에서 사용할 평균값 
	double sigma : 가우시안 필터에서 사용할 시그마값
-variables:
	Mat NoiseArr: 노이즈 추가한 이미지

2)Bilateralfilter_Gray
-parameter:
	const Mat input:노이즈를 제거할 이미지
	int n : 커널사이즈
	double sigma_t : bilateral filter에서 사용할 시그마t값
	double sigma_s : bilateral filter에서 사용할 시그마s값
	double sigma_r : bilateral filter에서 사용할 시그마s값
	const char *opt : bilateral filter에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel_spatial : spatial 담당 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	
	float kernelvalue : 최종 커널 값이 저장될 변수
	float G_sigma_r : intensity에 대한 커널 value
	float G_sigma_s : spatial에 대한 커널 value

	float w_ij : 픽셀에 대한 커널 value를 더한 값
	float w_st : 픽셀에 대한 커널 value에 input 픽셀값을 곱한 것을  더한 값
	Mat output : 노이즈 제거 이미지

3)Bilateralfilter_RGB
-parameter:
	const Mat input:노이즈를 제거할 이미지
	int n : 커널사이즈
	double sigma_t : bilateral filter에서 사용할 시그마t값
	double sigma_s : bilateral filter에서 사용할 시그마s값
	double sigma_r : bilateral filter에서 사용할 시그마s값
	const char *opt : bilateral filter에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
-variables:
	int row : 이미지의 행 개수
	int col : 이미지의 열 개수
	Mat kernel_spatial : spatial 담당 커널이 들어갈 변수
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	
	float kernelvalue : 최종 커널 값이 저장될 변수
	float G_sigma_r : intensity에 대한 커널 value
	float G_sigma_s : spatial에 대한 커널 value

	float w_ij[3]: 픽셀에 대한 커널 value를 채널별로 더한 값
	float w_st[3] : 픽셀에 대한 커널 value에 input 픽셀값을 곱한 것을 채널별로 더한 값
	Mat output : 노이즈 제거 이미지
4) main
-variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수	
	Mat noise_Gray : 회색이미지에 가우시안 노이즈가 적용된 이미지가 저장된 변수
 	Mat noise_RGB : 컬러이미지에 가우시안 노이즈가 적용된 이미지가 저장된 변수
	Mat Denoised_Gray :bilateral filter 적용 회색 이미지가 저장된 변수
	Mat Denoised_RGB : bilateral filter 적용 컬러 이미지가 저장된 변수



