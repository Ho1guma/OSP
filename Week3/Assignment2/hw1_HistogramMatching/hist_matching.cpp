#include "hist_func.h"
#include "plot.h"

void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF);
void inverse_func(G* trans_func, G* inversed_trans_func);
void combine_func(Mat& input, Mat& matched, G* trans_func, G* inversed_trans_func, G* trans_func_mt);


int main() {

	Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
	Mat reference = imread("reference.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;
	Mat refer_gray;

	cvtColor(input, input_gray, CV_RGB2GRAY);	
	cvtColor(reference, refer_gray, CV_RGB2GRAY);	

	Mat equalized_T = input_gray.clone();
	Mat equalized_G = refer_gray.clone();
	Mat matched = input_gray.clone();


	// PDF or transfer function txt files
	FILE *f_PDF;
	FILE * f_matched_PDF_gray;
	FILE *f_trans_func_mt;
	FILE* f_trans_func_eq_G;
	FILE* f_trans_func_eq_Gi;
	
	float* input_PDF = cal_PDF(input_gray);		
	float* input_CDF = cal_CDF(input_gray);				
	float* refer_CDF = cal_CDF(refer_gray);			

	fopen_s(&f_PDF, "PDF.txt", "w+");
	fopen_s(&f_matched_PDF_gray, "matched_PDF.txt", "w+");
	fopen_s(&f_trans_func_mt, "trans_func_mt.txt", "w+");
	fopen_s(&f_trans_func_eq_G, "trans_func_mt_G.txt", "w+");
	fopen_s(&f_trans_func_eq_Gi, "trans_func_mt_Gi.txt", "w+");

	G trans_func_eq_T[L] = { 0 };			// transfer function
	G trans_func_eq_G[L] = { 0 };
	G trans_func_eq_Gi[L] = { 0 };
	G trans_func_mt[L] = { 0 };

	hist_eq(input_gray, equalized_T, trans_func_eq_T, input_CDF);
	hist_eq(refer_gray, equalized_G, trans_func_eq_G, refer_CDF);

	inverse_func(trans_func_eq_G, trans_func_eq_Gi);
	combine_func(input_gray, matched, trans_func_eq_T, trans_func_eq_Gi, trans_func_mt);

	float* matched_PDF = cal_PDF(matched);

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF, "%d\t%f\n", i, input_PDF[i]);
		fprintf(f_matched_PDF_gray, "%d\t%f\n", i, matched_PDF[i]);
		// write transfer functions
		fprintf(f_trans_func_mt, "%d\t%d\n", i, trans_func_mt[i]);
		fprintf(f_trans_func_eq_G, "%d\t%d\n", i, trans_func_eq_G[i]);
		fprintf(f_trans_func_eq_Gi, "%d\t%d\n", i, trans_func_eq_Gi[i]);


	}


	funcDisplay(trans_func_mt, "func z");
	graphDisplay(input_PDF, "PDF");
	graphDisplay(matched_PDF, "PDF(hist_mt)");

	// memory release
	free(input_PDF);
	free(input_CDF);
	free(refer_CDF);
	fclose(f_PDF);
	fclose(f_matched_PDF_gray);
	fclose(f_trans_func_mt);
	fclose(f_trans_func_eq_G);
	fclose(f_trans_func_eq_Gi);


	////////////////////// Show each image ///////////////////////

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("Reference", WINDOW_AUTOSIZE);
	imshow("Reference", refer_gray);

	namedWindow("Matched", WINDOW_AUTOSIZE);
	imshow("Matched", matched);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

// histogram equalization
void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF) {

	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}

void inverse_func(G* trans_func, G* inversed_trans_func)
{
	for (int i = 0; i < L; i++)
	{
		inversed_trans_func[trans_func[i]] = i; //역함수
	}
	for (int i = 1; i < L; i++)//빈곳 채워주기
	{
		if (inversed_trans_func[i] == 0) 
			inversed_trans_func[i] = inversed_trans_func[i -1];
	}
	
}

void combine_func(Mat &input, Mat &matched, G *trans_func, G* inversed_trans_func, G* trans_func_mt) {

	for (int i = 0; i < L; i++)
	{
		trans_func_mt[i] = inversed_trans_func[trans_func[i]]; 
	}
	for (int i = 1; i < L; i++) //빈곳채워주기
	{
		if (trans_func_mt[i] == 0)
			trans_func_mt[i] = trans_func_mt[i - 1];
	}

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
		{
			matched.at<G>(i, j) = trans_func_mt[input.at<G>(i, j)];
		}
}