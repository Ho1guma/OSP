1. GaussianFilter_separable.cpp

- Purpose of this code
컬러, 회색이미지에 대해 separable한 방식으로 가우시안 필터를 적용한 이미지를 window 창에 display해주는 코드이다.

1) gaussianfilter:
-parameter:
	const Mat input : 원본 이미지 
	int n : 커널 사이즈 2n+1에서의 n
	float sigmaT : 가우시안 필터에서 사용할 시그마t
	float sigmaS : 가우시안 필터에서 사용할 시그마s
	const char* opt : 가우시안 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
- variables:
	Mat kernel : 전체 커널이 들어갈 변수
	Mat W_s : 커널 중 W_s 값
	Mat W_t : 커널 중 W_t 값
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	float denom_s : W_s 를 유도할 때 사용할 분모값
	float denom_t : W_t 를 유도할 때 사용할 분모값 
	Mat temp : input에 대해 W_t를 적용해 저장한 임시값
	Mat output : temp에 대해 W_s를 적용해 나온 최종 커널 적용값

2) main
- variables:
	clock_t start_g, end_g, start_c, end_c : 회색 이미지, 컬러이미지에 대한 가우시안 필터 적용 전, 후 시각
	double result_g, result_c : 회색이미지, 컬러이미지에 대한 가우시안필터 적용 시간
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수
	Mat output : 가우시안 필터 적용 컬러 이미지가 저장된 변수
	Mat output_gray : 가우시안 필터 적용 회색이미지가 저장된 변수

2. UnsharpMasking.cpp

- Purpose of this code
컬러, 회색이미지에 대해 UnsharpMarking을 적용한 이미지를 window 창에 display해주는 코드이다.

1) gaussianfilter
-parameter:
	const Mat input : 원본 이미지 
	int n : 커널 사이즈 2n+1에서의 n
	float sigmaT : 가우시안 필터에서 사용할 시그마t
	float sigmaS : 가우시안 필터에서 사용할 시그마s
	const char* opt : 가우시안 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
- variables:
	Mat kernel : 전체 커널이 들어갈 변수
	Mat W_s : 커널 중 W_s 값
	Mat W_t : 커널 중 W_t 값
	int kernel_size : 커널 사이즈 
	int tempa : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	int tempb : image boundary 픽셀의 조정된 픽셀값이 들어갈 변수
	float denom_s : W_s 를 유도할 때 사용할 분모값
	float denom_t : W_t 를 유도할 때 사용할 분모값 
	Mat temp : input에 대해 W_t를 적용해 저장한 임시값
	Mat output : temp에 대해 W_s를 적용해 나온 최종 커널 적용값

2) UnsharpMask
-parameter:
	const Mat input : 원본 이미지 
	int n : 커널 사이즈 2n+1에서의 n
	float sigmaT : 가우시안 필터에서 사용할 시그마t
	float sigmaS : 가우시안 필터에서 사용할 시그마s
	const char* opt : 가우시안 필터에서 image boundary 픽셀의 처리방식("zero-paddle", "mirroring","adjustkernel")
	float k : unsharpmasking에서 사용할 k값
-variables:
	Mat L : 이미지에 대해 가우시안 필터를 적용한 값
	Mat output : 이미지에 대해 unsharpmasking을 적용한 값

3) main
- variables:
	Mat input : 원본 컬러 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수
	Mat output : 가우시안 필터 적용 컬러 이미지가 저장된 변수
	Mat output_gray : 가우시안 필터 적용 회색이미지가 저장된 변수