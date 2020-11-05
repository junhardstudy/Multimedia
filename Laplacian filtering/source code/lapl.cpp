#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void FindZeroCrossings(Mat& laplacian, Mat& zero_crossings);

void main() {
	/************************
	Mat타입이 애초에 생성될때
	16비트 or 32비트로 생성되서 그럴수도
	있음
	*************************/
	int i;
	double sigma = 1;
	Mat img;
	Mat gray_img;
	Mat gaussian_img;
	Mat laplacian_img;
	Mat Abs_laplacian_img;
	Mat zero_crossing_lapl;
	Mat Abs_zero_crossing;
	Mat road_img;
	Mat gray_road;
	Mat blur_road;
	Mat lapl_road;
	Mat zero_road;
	Mat hough;
	Mat abs_road;
	String str1("Gaussian_result_");
	String str2("Laplacian_result_");
	String str3("Zero_crossing_");
	String str4("Road");
	String str5("Road_lapl");
	String format_name(".jpg");
	String tmp1;
	String tmp2;
	String tmp3;
	char version[2] = "0";


	img = imread("Lenna.png", IMREAD_COLOR);
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	imwrite("gray image.jpg", gray_img);


	for (i = 1; i < 6; i++) {
		sigma = i;
		GaussianBlur(gray_img, gaussian_img, Size(5, 5), sigma, 0, BORDER_DEFAULT);
		Laplacian(gaussian_img, laplacian_img, CV_32F, 3, 1, 0, BORDER_DEFAULT);
		FindZeroCrossings(laplacian_img, zero_crossing_lapl);
		convertScaleAbs(zero_crossing_lapl, Abs_zero_crossing);

		version[0] = '0' + i;
		String ver(version);

		tmp1 = str1 + ver + format_name;
		tmp2 = str2 + ver + format_name;
		tmp3 = str3 + ver + format_name;

		imwrite(tmp1, gaussian_img);
		imwrite(tmp2, laplacian_img);
		imwrite(tmp3, Abs_zero_crossing);

	}
	road_img = imread("road.jpg", IMREAD_COLOR);
	cvtColor(road_img, gray_road, COLOR_BGR2GRAY);


	for (i = 1; i < 6; i++) {
		version[0] = '0' + i;
		String ver(version);

		GaussianBlur(gray_road, blur_road, Size(5, 5), sigma, 0, BORDER_DEFAULT);
		Laplacian(blur_road, lapl_road, CV_32F, 3, 1, 0, BORDER_DEFAULT);
		FindZeroCrossings(lapl_road, zero_road);
		convertScaleAbs(zero_road, abs_road);

		vector<Vec4i> lines;
		int threshold = 70;
		HoughLinesP(abs_road, lines, 1, CV_PI / 180, threshold);
		hough = road_img.clone();
		for (size_t j = 0; j < lines.size(); j++) {
			Vec4i l = lines[j];
			line(hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1);
		}
		tmp1 = str4 + ver + format_name;
		tmp2 = str5 + ver + format_name;
		imwrite(tmp2, lapl_road);
		imwrite(tmp1, hough);
		/*
		imwrite("road_gray_blur.jpg", blur_road);
		imwrite("road_lapl.jpg", lapl_road);
		imwrite("road_zero.jpg", zero_road);
		imwrite("line_detection.jpg", hough);
		*/
	}



	waitKey(0);

}

void FindZeroCrossings(Mat& laplacian, Mat& zero_crossings)
{
	Mat* result = new Mat(laplacian.size(), CV_8U, Scalar(0));
	zero_crossings = *result;
	int image_rows = laplacian.rows;
	int image_channels = laplacian.channels();
	int values_on_each_row = laplacian.cols * image_channels;
	float laplacian_threshold = 16;
	// Find Zero Crossings

	for (int row = 1; row < image_rows; row++) {
		float* prev_row_pixel = laplacian.ptr<float>(row - 1) + 1;
		float* curr_row_pixel = laplacian.ptr<float>(row);
		uchar* output_pixel = zero_crossings.ptr<uchar>(row) + 1;
		for (int column = 1; column < values_on_each_row; column++)
		{
			float prev_value_on_row = *curr_row_pixel;
			curr_row_pixel++;
			float curr_value = *curr_row_pixel;
			float prev_value_on_column = *prev_row_pixel;
			float difference = 0.0;
			if (((curr_value > 0) && (prev_value_on_row < 0)) ||
				((curr_value < 0) && (prev_value_on_row > 0)))
				difference = abs(curr_value - prev_value_on_row);
			if ((((curr_value > 0) && (prev_value_on_column < 0)) ||
				((curr_value < 0) && (prev_value_on_column > 0))) &&
				(abs(curr_value - prev_value_on_column) > difference))
				difference = abs(curr_value - prev_value_on_column);
			*output_pixel = (difference > laplacian_threshold) ? 255 : 0;// (int) ((100 * difference) / laplacian_threshold);
			prev_row_pixel++;
			output_pixel++;
		}
	}
}