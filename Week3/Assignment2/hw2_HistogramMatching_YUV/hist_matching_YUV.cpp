#include "hist_func.h"
#include "plot.h"

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void inverse_func(G* trans_func, G* inversed_trans_func);
void combine_func(Mat& input, Mat& matched, G* trans_func, G* inversed_trans_func, G* trans_func_mt);


int main() {

	Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
	Mat reference = imread("reference.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_equalized;
	Mat refer_equalized;
	Mat matched = input.clone();

	cvtColor(input, input_equalized, CV_RGB2YUV);	// RGB -> YUV
	cvtColor(reference, refer_equalized, CV_RGB2YUV);	// RGB -> YUV

	// split each channel(Y, U, V)
	Mat input_channels[3];
	split(input_equalized, input_channels);
	Mat input_Y = input_channels[0];						// U = channels[1], V = channels[2]

	// split each channel(Y, U, V)
	Mat refer_channels[3];
	split(refer_equalized, refer_channels);
	Mat refer_Y = refer_channels[0];						// U = channels[1], V = channels[2]


	// PDF or transfer function txt files
	FILE* f_PDF_RGB;
	FILE* f_matched_PDF_YUV;
	FILE* f_trans_func_mt_YUV;

	float** input_PDF_RGB = cal_PDF_RGB(input);		// PDF of Input image(RGB) : [L][3]
	float* input_CDF_YUV = cal_CDF(input_Y);				// CDF of Y channel image
	float* refer_CDF_YUV = cal_CDF(refer_Y);				// CDF of Y channel image

	fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
	fopen_s(&f_matched_PDF_YUV, "matched_PDF_YUV.txt", "w+");
	fopen_s(&f_trans_func_mt_YUV, "trans_func_mt_YUV.txt", "w+");

	G trans_func_eq_T[L] = { 0 };			// transfer function
	G trans_func_eq_G[L] = { 0 };
	G trans_func_eq_Gi[L] = { 0 };
	G trans_func_mt_YUV[L] = { 0 };

	// histogram equalization on Y channel
	hist_eq(input_Y, input_channels[0], trans_func_eq_T, input_CDF_YUV);
	hist_eq(refer_Y, refer_channels[0], trans_func_eq_G, refer_CDF_YUV);

	inverse_func(trans_func_eq_G, trans_func_eq_Gi);
	combine_func(input_Y, input_channels[0], trans_func_eq_T, trans_func_eq_Gi, trans_func_mt_YUV);

	// merge Y, U, V channels
	merge(input_channels, 3, matched);

	// YUV -> RGB (use "CV_YUV2RGB" flag)
	cvtColor(matched, matched, CV_YUV2RGB);

	// equalized PDF (YUV)
	float** matched_PDF_YUV = cal_PDF_RGB(matched);


	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF_RGB, "%d\t%f\t%f\t%f\n", i, input_PDF_RGB[i][0], input_PDF_RGB[i][1], input_PDF_RGB[i][2]);
		fprintf(f_matched_PDF_YUV, "%d\t%f\t%f\t%f\n", i, matched_PDF_YUV[i][0],
			matched_PDF_YUV[i][1], matched_PDF_YUV[i][2]);
		// write transfer functions
		fprintf(f_trans_func_mt_YUV, "%d\t%d\n", i, trans_func_mt_YUV[i]);


	}

	funcDisplay(trans_func_mt_YUV, "func z");
	// memory release
	free(input_PDF_RGB);
	free(input_CDF_YUV);
	free(refer_CDF_YUV);
	fclose(f_PDF_RGB);
	fclose(f_matched_PDF_YUV);
	fclose(f_trans_func_mt_YUV);


	////////////////////// Show each image ///////////////////////

	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", input);

	namedWindow("Reference", WINDOW_AUTOSIZE);
	imshow("Reference", reference);

	namedWindow("Matched", WINDOW_AUTOSIZE);
	imshow("Matched", matched);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

// histogram equalization
void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

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
			inversed_trans_func[i] = inversed_trans_func[i - 1];
	}

}

void combine_func(Mat& input, Mat& matched, G* trans_func, G* inversed_trans_func, G* trans_func_mt) {

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