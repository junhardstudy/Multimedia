#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main() {
	int i, j, t;
	int error;
	Mat img;
	Mat gray_img;
	Mat after_img;
	Mat after_col_img;

	img = imread("Lenna.png", IMREAD_COLOR);
	after_col_img = imread("Lenna.png", IMREAD_COLOR);

	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	cvtColor(img, after_img, COLOR_BGR2GRAY);
	imwrite("gray_image.jpg", gray_img);

	/************************************************************************
		gray image에 대해 error diffusion dithering algorithm을 적용합니다.
	*************************************************************************/
	for (i = 0; i < after_img.rows - 1; i++) {
		for (j = 1; j < after_img.cols - 1; j++) {
			if (after_img.at<uchar>(i, j) > 127)error = after_img.at<uchar>(i, j) - 255;
			else error = after_img.at<uchar>(i, j);
			after_img.at<uchar>(i, j + 1) += (uchar)((error * 7) / 16);
			after_img.at<uchar>(i + 1, j - 1) += (uchar)((error * 3) / 16);
			after_img.at<uchar>(i + 1, j) += (uchar)((error * 5) / 16);
			after_img.at<uchar>(i + 1, j + 1) += (uchar)((error * 1) / 16);
		}
	}
	for (i = 0; i < after_img.rows; i++) {
		for (j = 0; j < after_img.cols; j++) {
			if (after_img.at<uchar>(i, j) <= 127)after_img.at<uchar>(i, j) = 0;
			else after_img.at<uchar>(i, j) = 255;
		}
	}

	imwrite("result.jpg", after_img);

	/************************************************************************
		color image에 대해 error diffusion dithering algorithm을 적용합니다.
	*************************************************************************/

	for (t = 0; t < 3; t++) {
		for (i = 0; i < after_col_img.rows - 1; i++) {
			for (j = 1; j < after_col_img.cols - 1; j++) {

				if (after_col_img.at<Vec3b>(i, j)[t] > 127)error = after_col_img.at<Vec3b>(i, j)[t] - 255;
				else error = after_col_img.at<Vec3b>(i, j)[t];
				after_col_img.at<Vec3b>(i, j + 1)[t] += (error * 7) / 16;
				after_col_img.at<Vec3b>(i + 1, j - 1)[t] += (error * 3) / 16;
				after_col_img.at<Vec3b>(i + 1, j)[t] += (error * 5) / 16;
				after_col_img.at<Vec3b>(i + 1, j + 1)[t] += (error * 1) / 16;
			}

		}
	}


	for (t = 0; t < 3; t++) {
		for (i = 0; i < after_col_img.rows; i++)
			for (j = 0; j < after_col_img.cols; j++) {
				if (after_col_img.at<Vec3b>(i, j)[t] <= 127) after_col_img.at<Vec3b>(i, j)[t] = 0;
				else after_col_img.at<Vec3b>(i, j)[t] = 255;
			}
	}


	namedWindow("original_dip", WINDOW_NORMAL);
	namedWindow("gray_dip", WINDOW_NORMAL);
	namedWindow("after_dip", WINDOW_NORMAL);
	namedWindow("after_col_dip", WINDOW_NORMAL);
	imshow("original_dip", img);
	imshow("gray_dip", gray_img);
	imshow("after_dip", after_img);
	imshow("after_col_dip", after_col_img);
	imwrite("result_col.jpg", after_col_img);
	waitKey(0);



	return 0;
}