#pragma once
#include "hist_func.h"

void graphDisplay(float data[], const char* name)
{
	int hist_w = 512;  //  폭
	float hist_h = 200;  // 높이
	int count = 256; //데이터의 개수
	int bin_w = hist_w / count;

	float max = data[0];
	for (int i = 0; i < 255; i++) {
		if (max < data[i])
			max = data[i];
	}
	float scale;
	if (max != 0)
		scale = round(200 / max);
	else
		scale = 1;

	Mat graphImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < 255; i++) {
		line(graphImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - data[i] * scale), Scalar(255, 0, 0));
	}
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, graphImage);

}
void funcDisplay(G data[], const char* name)
{
	int hist_w = 512;  //  폭
	float hist_h = 200;  // 높이
	int count = 256; //데이터의 개수
	int bin_w = hist_w / count;

	float max = data[0];
	for (int i = 0; i < 255; i++) {
		if (max < data[i])
			max = data[i];
	}
	float scale;
	if (max != 0)
		scale = round(200 / max);
	else
		scale = 1;

	Mat graphImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < 254; i++) {
		line(graphImage, Point(bin_w * (i), hist_h - data[i] * scale),
			Point(bin_w * (i + 1), hist_h - data[i + 1] * scale), Scalar(255, 0, 0));
	}
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, graphImage);

}