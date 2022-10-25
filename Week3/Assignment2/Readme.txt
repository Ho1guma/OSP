1. plot.h

- Purpose of this code
이미지에 대한 PDF 데이터와 변환 함수를 윈도우 창에 시각화(각각 막대그래프, 꺾은선 그래프)하기 위한 헤더파일이다.

1) graphDisplay
-parameter :
	float data[] : 256개의 PDF 데이터가 저장된 배열
	const char* name : 윈도우창의 제목이 될 문자열
-variables:
	int hist_w : 윈도우 창의 폭
	float hist_h : 윈도우 창의 높이
	int count : 데이터의 개수(256개 고정)
	int bin_w : 데이터 간의 폭 길이
	float max : 데이터 중 가장 큰 값
	scale : 데이터 시각화를 위해 배수가 될 값(스케일)
2) funcDisplay
-parameter:
	G data[] : trans 함수의 y값이 되는 배열
	const char* name : 윈도우창의 제목이 될 문자열
-variables:
	int hist_w : 윈도우 창의 폭
	float hist_h : 윈도우 창의 높이
	int count : 데이터의 개수(256개 고정)
	int bin_w : 데이터 간의 폭 길이
	float max : 데이터 중 가장 큰 값
	scale : 데이터 시각화를 위해 배수가 될 값(스케일)

2. hist_matching.cpp

- Purpose of this code
input이미지에 대해 회색이미지로 변환 후 reference 이미지를  이용해 histogram matching을 수행하여 
histogram matched image를 반환하는 코드이다. 

1) hist_eq
-parameter:
	Mat &input : 원본 회색 이미지
	Mat &equalized : equalized 이미지가 저장될 변수
	G *trans_func : equalized 과정에서 사용된 trans_function가 저장될 변수
	float *CDF : equalized 과정에서 필요한 원본 회색 이미지의 CDF 값

2) inverse_func
-parameter:
	G* trans_func : equalized 과정에서 사용된 trans_function
	G* inversed_trans_func : trans_func의 역함수가 저장될 변수

3) combine_func
-parameter:
	Mat &input : 원본 회색 이미지
	Mat &matched : histogram matched 이미지가 저장될 변수
	G *trans_func : equalized 과정에서 사용된 trans_function
	G* inversed_trans_func: trans_func의 역함수
	G* trans_func_mt : histogram match에 사용된 trans_func이 저장될 변수
4) main
- variables:
	Mat input : 원본 이미지가 저장된 변수
	Mat reference : reference 이미지가 저장된 변수
	Mat input_gray : 원본 회색이미지가 저장된 변수
	Mat refer_gray : reference 회색이미지가 저장된 변수
	Mat equalized_T : 원본 회색이미지의 equalized 이미지
	Mat equalized_G : reference 회색이미지의 equalized 이미지
	Mat matched: 원본회색이미지의 histogram matched 이미지
	FILE *f_PDF : 원본 회색 이미지의 PDF 값이 저장될 파일 포인터
	FILE * f_matched_PDF_gray : matched 이미지의 PDF값이 저장될 파일 포인터
	FILE *f_trans_func_mt : histogram matched 과정에서 사용된 trans_func이 저장된 파일 포인터
	FILE* f_trans_func_eq_G : reference 회색이미지의 equalized 과정에서 사용된 trans_func이 저장된 파일 포인터
	FILE* f_trans_func_eq_Gi : reference 회색이미지의 equalized 과정에서 사용된 trans_func의 역함수가 저장된 파일 포인터
	float* input_PDF : 원본 회색이미지의 PDF 값을 저장한 변수	
	float* input_CDF : 원본 회색이미지의 CDF 값을 저장한 변수	
	float* refer_CDF : reference 회색이미지의 PDF 값을 저장한 변수	
	float* matched_PDF : matched 원본 회색이미지의 PDF 값을 저장한 변수
	G trans_func_eq_T[L] : 원본 회색이미지의 equalized 과정에서 사용된 trans_func의 y값들이 저장된 배열
	G trans_func_eq_G[L] : reference 회색이미지의 equalized 과정에서 사용된 trans_func의 y값들이 저장된 배열
	G trans_func_eq_Gi[L] : reference 회색이미지의 equalized 과정에서 사용된 trans_func의 역함수의 y값들이 저장된 배열
	G trans_func_mt[L] : 원본 회색이미지의 histogram matched 과정에서 사용된 trans_func의 y값들이 저장된 배열
	
3. hist_matching_YUV.cpp

- Purpose of this code
input이미지에 대해 YUV이미지로 변환 후 reference 이미지를 이용해 YUV에 대한 histogram matching을 수행하여 
histogram matched image를 반환하는 코드이다. 

1) hist_eq
-parameter:
	Mat &input : 원본 회색 이미지
	Mat &equalized : equalized 이미지가 저장될 변수
	G *trans_func : equalized 과정에서 사용된 trans_function가 저장될 변수
	float *CDF : equalized 과정에서 필요한 원본 회색 이미지의 CDF 값

2) inverse_func
-parameter:
	G* trans_func : equalized 과정에서 사용된 trans_function
	G* inversed_trans_func : trans_func의 역함수가 저장될 변수

3) combine_func
-parameter:
	Mat &input : 원본 회색 이미지
	Mat &matched : histogram matched 이미지가 저장될 변수
	G *trans_func : equalized 과정에서 사용된 trans_function
	G* inversed_trans_func: trans_func의 역함수
	G* trans_func_mt : histogram match에 사용된 trans_func이 저장될 변수
4) main
- variables:
	Mat input : 원본 이미지가 저장된 변수
	Mat reference : reference 이미지가 저장된 변수
	Mat input_equalized: 원본 YUV이미지의 equalized 이미지
	Mat refer_equalized: reference YUV이미지의 equalized 이미지
	Mat matched: 원본이미지의 histogram matched 이미지
	Mat input_channels[3] : 원본 이미지의 YUV 채널을 담은 배열 변수
	Mat input_Y : 원본이미지의 Y채널만을 담은 변수
	Mat refer_channels[3] : reference 이미지의 YUV 채널을 담은 배열 변수
	Mat refer_Y : reference 이미지의 Y채널만을 담은 변수
	FILE *f_PDF_RGB : 원본 RGB 이미지의 PDF 값이 저장될 파일 포인터
	FILE * f_matched_PDF_YUV : matched 이미지의 PDF값이 저장될 파일 포인터
	FILE *f_trans_func_mt_YUV : histogram matched 과정에서 사용된 trans_func이 저장된 파일 포인터
	float* input_PDF_RGB : 원본 RGB이미지의 PDF 값을 저장한 변수	
	float* input_CDF_YUV : 원본 YUV이미지의 Y채널의 CDF 값을 저장한 변수	
	float* refer_CDF_YUV : reference 회색이미지의 Y채널의 CDF 값을 저장한 변수	
	float* matched_PDF_YUV : matched 원본 이미지의 PDF 값을 저장한 변수
	G trans_func_eq_T[L] : 원본 이미지의 equalized 과정에서 사용된 trans_func의 y값들이 저장된 배열
	G trans_func_eq_G[L] : reference 이미지의 equalized 과정에서 사용된 trans_func의 y값들이 저장된 배열
	G trans_func_eq_Gi[L] : reference 이미지의 equalized 과정에서 사용된 trans_func의 역함수의 y값들이 저장된 배열
	G trans_func_mt[L] : 원본 이미지의 histogram matched 과정에서 사용된 trans_func의 y값들이 저장된 배열
	
