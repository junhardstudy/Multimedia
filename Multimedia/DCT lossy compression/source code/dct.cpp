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

	�ڷ� ������
	DCT image bsis block�� 8X8 ������ ��,

			[1][2][3][4][5][6][7][8]
			[9][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]
			[.][.][.][.][.][.][.][.]

	[1]�迭 �ȿ��� �ϳ��� DCT basis matrix�� ǥ���ϱ� ����
	�ٽ� 2���� �迭�� �ּҸ� �����ϵ��� �س����ϴ�.
	�� [1]�� �����ϸ� [1]�迭 �ȿ��� �ٸ� 2���� �迭�� �����մϴ�.

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
	float** block[8][8];//DCT �������� ��ü 64���� �׽��ϴ�.
	float** eachblock;//������ DTC���ȿ��� (i, j) �� (0, 0)�������� (8, 8)���� �ִ� 64���� ���Ҹ� ���� 8X8 ����� 2���� �迭�� ��Ÿ�½��ϴ�. 
	float** readblock;//block[][]���� �迭�� �б⸸ �ϴ� �����Դϴ�.
	float tmp = 0;
	int i, j, u, v;
	int l, k;//�ݺ��� ���� ����
	double Cu;
	double Cv;
	//unsigned char test[8][8] = { {182, 196, 199, 201, 203, 201, 199, 173}, {175, 180, 176, 142, 148, 152, 148, 120}, {148, 118, 123, 115, 114, 107, 108, 107}, {115, 110, 110, 112, 105, 109, 101, 100}, {104, 106, 106, 102, 104, 95, 98, 105}, {99, 115, 131, 104, 118, 86, 87, 133}, {112, 154, 154, 107, 140, 97, 88, 151}, {145, 158, 178, 123, 132, 140 ,138, 133} };
	float result[8][8] = { {0} };//�ϳ��� DCT basis���� ���̹������� 8by8�� ������ ���� ���� DCT�� ������ ����� ���� �ӽ� 2���� �迭
	Mat img;//���̹����� ����  CV::Mat ��ü �Դϴ�.
	Mat gray_img;
	Mat mat(100, 100, CV_8U);
	Mat mat1(2, 2, CV_8U), mat2(3, 3, CV_8U);
	Mat tmp_mat(8, 8, CV_8U);
	Mat tmp_result(8, 8, CV_64F);//8by8 ���ļ� ������ ���� �ӽ� ����


	img = imread("lena.jpg", IMREAD_COLOR);
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	Mat result8by8(gray_img.rows, gray_img.cols, CV_8U);//IDCT���� 8by8�� ����� ��� �̹�����
	Mat result4by4(gray_img.rows, gray_img.cols, CV_8U);//IDCT���� 4by4�� ����� ��� �̹�����
	Mat result2by2(gray_img.rows, gray_img.cols, CV_8U);//IDCT���� 2by2�� ����� ��� �̹�����


	printf("Converting images...");

	//DCT basis function�� set�� ����� �ܰ��Դϴ�.	
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


				}//�ϳ��� Base �̹��� DCT matrix
			}
			block[u][v] = eachblock;
			//������ Base �̹���( Base(u, v) matrix)
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//apply DCT to image matrix

	for (l = 0; l < gray_img.rows; l += 8) {
		for (k = 0; k < gray_img.cols; k += 8) {
			//���⿡ dct , idct ����
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					tmp_mat.at<uchar>(i, j) = gray_img.at<uchar>(l + i, k + j);
				}
			}//�����̹����� �Ϻκ��� 8by8�� ��������

			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++)tmp_result.at<double>(i, j) = 0;//���ļ� ������ ������� ���� �ӽ� �迭�� �ʱ�ȭ
			}
			/*******************************������� original image�� 8by8 ��Ʈ���� ����********************************/


			for (u = 0; u < 8; u++) {
				for (v = 0; v < 8; v++) {
					readblock = block[u][v];
					for (i = 0; i < 8; i++) {
						for (j = 0; j < 8; j++) {
							tmp_result.at<double>(u, v) += readblock[i][j] * tmp_mat.at<uchar>(i, j);
							//�����̹����� 8by8�� �����°Ͱ� basis image�� ����
						}
					}

				}
			}
			//Mat::tmp_result��ü�� DCT�� ����� �����


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

//DCT basis ���� Ȯ���ϱ� ���� cmdâ�� ��� �� �ִ� �Լ��Դϴ�.
void showDCT(float** basis, int num) {
	int v, u;

	printf("====================DCT basis matrix(0, %d) �� ���====================\n", num);
	for (v = 0; v < 8; v++) {
		for (u = 0; u < 8; u++) {
			printf("%8.3f ", basis[v][u]);
		}
		printf("\n");
	}
	printf("\n\n");
}

//�� �̹��� �ȼ� ��ǥ(i, j)�� IDCT�� ����� ������� return ���ִ� �Լ��Դϴ�. 
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