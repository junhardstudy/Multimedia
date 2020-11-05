#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#define pi 3.141592

using namespace cv;
using namespace std;

char IDCT(Mat DCTed, int i, int j, int mode, float** basis);
void showDCT(float** basis, int);
/********************************************************************

	자료 구조는
	DCT image bsis block이 8X8 존재할 때,

			[1][2][3][4][5][6][7][8]
			[9][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]

	[1]배열 안에는 하나의 DCT basis matrix를 표현하기 위해
	다시 2차원 배열의 주소를 참조하도록 해놨습니다.
	즉 [1]을 참조하면 [1]배열 안에는 다른 2차원 배열이 존재합니다.

	[1] => [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]
		   [0.125][0.125][0.125][0.125][0.125][0.125][0.125][0.125]


*********************************************************************/
void main() {
	float** block[8][8];//DCT 각각블럭이 전체 64개를 뒀습니다.
	float** eachblock;//각각의 DTC블럭안에는 (i, j) 가 (0, 0)에서부터 (8, 8)까지 있는 64개의 원소를 가진 8X8 행렬을 2차원 배열로 나타냈습니다. 
	float** readblock;//block[][]안의 배열을 읽기만 하는 변수입니다.
	float tmp = 0;
	int i, j, u, v;
	int l, k;//반복문 제어 변수
	double Cu;
	double Cv;
	//unsigned char test[8][8] = { {182, 196, 199, 201, 203, 201, 199, 173}, {175, 180, 176, 142, 148, 152, 148, 120}, {148, 118, 123, 115, 114, 107, 108, 107}, {115, 110, 110, 112, 105, 109, 101, 100}, {104, 106, 106, 102, 104, 95, 98, 105}, {99, 115, 131, 104, 118, 86, 87, 133}, {112, 154, 154, 107, 140, 97, 88, 151}, {145, 158, 178, 123, 132, 140 ,138, 133} };
	float result[8][8] = { {0} };//하나의 DCT basis블럭과 원이미지에서 8by8만 가져온 블럭에 대해 DCT를 적용한 결과를 담을 임시 2차원 배열
	Mat img;//원이미지를 담을  CV::Mat 객체 입니다.
	Mat gray_img;
	Mat mat(100, 100, CV_8U);
	Mat mat1(2, 2, CV_8U), mat2(3, 3, CV_8U);
	Mat tmp_mat(8, 8, CV_8U);
	Mat tmp_result(8, 8, CV_64F);//8by8 주파수 영역을 담을 임시 변수


	img = imread("lena.jpg", IMREAD_COLOR);
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	Mat result8by8(gray_img.rows, gray_img.cols, CV_8U);//IDCT에서 8by8만 사용한 결과 이미지값
	Mat result4by4(gray_img.rows, gray_img.cols, CV_8U);//IDCT에서 4by4만 사용한 결과 이미지값
	Mat result2by2(gray_img.rows, gray_img.cols, CV_8U);//IDCT에서 2by2만 사용한 결과 이미지값


	printf("Converting images...");

	//DCT basis function과 set을 만드는 단계입니다.	
	for (u = 0; u < 8; u++) {
		for (v = 0; v < 8; v++) {
			eachblock = (float**)malloc(sizeof(float*) * 8);
			for (i = 0; i < 8; i++) {
				eachblock[i] = (float*)malloc(sizeof(float) * 8);
				for (j = 0; j < 8; j++) {
					if (u == 0)Cu = 1 / sqrt(2.0);
					else Cu = 1;
					if (v == 0)Cv = 1 / sqrt(2.0);
					else Cv = 1;

					eachblock[i][j] = ((Cu * Cv) / 4.0) * cos(((2 * i + 1) * u * pi) / 16.0) * cos(((2 * j + 1) * v * pi) / 16.0);


				}//하나의 Base 이미지 DCT matrix
			}
			block[u][v] = eachblock;
			//각각의 Base 이미지( Base(u, v) matrix)
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//apply DCT to image matrix

	for (l = 0; l < gray_img.rows; l += 8) {
		for (k = 0; k < gray_img.cols; k += 8) {
			//여기에 dct , idct 수행
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					tmp_mat.at<uchar>(i, j) = gray_img.at<uchar>(l + i, k + j);
				}
			}//원본이미지의 일부분인 8by8만 가져오기

			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++)tmp_result.at<double>(i, j) = 0;//주파수 도메인 결과값을 담을 임시 배열을 초기화
			}
			/*******************************여기까지 original image의 8by8 매트릭스 추출********************************/


			for (u = 0; u < 8; u++) {
				for (v = 0; v < 8; v++) {
					readblock = block[u][v];
					for (i = 0; i < 8; i++) {
						for (j = 0; j < 8; j++) {
							tmp_result.at<double>(u, v) += readblock[i][j] * tmp_mat.at<uchar>(i, j);
							//원본이미지의 8by8만 가져온것과 basis image를 내적
						}
					}

				}
			}
			//Mat::tmp_result객체는 DCT가 적용된 결과물


			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					result8by8.at<uchar>(l + i, k + j) = IDCT(tmp_result, i, j, 8, block[i][j]);
					result4by4.at<uchar>(l + i, k + j) = IDCT(tmp_result, i, j, 4, block[i][j]);
					result2by2.at<uchar>(l + i, k + j) = IDCT(tmp_result, i, j, 2, block[i][j]);
				}
			}


		}

	}


	imwrite("8by8.jpg", result8by8);
	imwrite("4by4.jpg", result4by4);
	imwrite("2by2.jpg", result2by2);
	printf("\n\n");
	for (u = 0; u < 8; u++)showDCT(block[0][u], u);

}

//DCT basis 값을 확인하기 위해 cmd창에 출력 해 주는 함수입니다.
void showDCT(float** basis, int num) {
	int v, u;

	printf("====================DCT basis matrix(0, %d) 값 출력====================\n", num);
	for (v = 0; v < 8; v++) {
		for (u = 0; u < 8; u++) {
			printf("%8.3f ", basis[v][u]);
		}
		printf("\n");
	}
	printf("\n\n");
}

//원 이미지 픽셀 좌표(i, j)에 IDCT를 계산한 결과값을 return 해주는 함수입니다. 
char IDCT(Mat DCTed, int i, int j, int mode, float** basis) {
	double result;
	double Cv, Cu;
	int u, v, m, n;
	result = 0;
	for (u = 0; u < mode; u++) {
		for (v = 0; v < mode; v++) {
			if (u == 0)Cu = 1 / sqrt(2.0);
			else Cu = 1;
			if (v == 0)Cv = 1 / sqrt(2.0);
			else Cv = 1;
			//result += basis[u][v] * DCTed.at<double>(u, v);
			result += ((Cu * Cv) / 4.0) * cos(((2 * i + 1) * u * pi) / 16.0) * cos(((2 * j + 1) * v * pi) / 16.0) * DCTed.at<double>(u, v);
		}
	}


	return (char)result;
}