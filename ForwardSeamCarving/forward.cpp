#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
std::string GetMatType(const cv::Mat& mat)
{
	const int mtype = mat.type();

	switch (mtype)
	{
	case CV_8UC1:  return "CV_8UC1";
	case CV_8UC2:  return "CV_8UC2";
	case CV_8UC3:  return "CV_8UC3";
	case CV_8UC4:  return "CV_8UC4";

	case CV_8SC1:  return "CV_8SC1";
	case CV_8SC2:  return "CV_8SC2";
	case CV_8SC3:  return "CV_8SC3";
	case CV_8SC4:  return "CV_8SC4";

	case CV_16UC1: return "CV_16UC1";
	case CV_16UC2: return "CV_16UC2";
	case CV_16UC3: return "CV_16UC3";
	case CV_16UC4: return "CV_16UC4";

	case CV_16SC1: return "CV_16SC1";
	case CV_16SC2: return "CV_16SC2";
	case CV_16SC3: return "CV_16SC3";
	case CV_16SC4: return "CV_16SC4";

	case CV_32SC1: return "CV_32SC1";
	case CV_32SC2: return "CV_32SC2";
	case CV_32SC3: return "CV_32SC3";
	case CV_32SC4: return "CV_32SC4";

	case CV_32FC1: return "CV_32FC1";
	case CV_32FC2: return "CV_32FC2";
	case CV_32FC3: return "CV_32FC3";
	case CV_32FC4: return "CV_32FC4";

	case CV_64FC1: return "CV_64FC1";
	case CV_64FC2: return "CV_64FC2";
	case CV_64FC3: return "CV_64FC3";
	case CV_64FC4: return "CV_64FC4";

	default:
		return "Invalid type of matrix!";
	}
}
//Forward Seam Carving
int main() {
	Mat img = imread("balloon.jpg", IMREAD_COLOR);
	namedWindow("Landscape", WINDOW_AUTOSIZE);
	//namedWindow("Best Seam", WINDOW_AUTOSIZE);
	//namedWindow("M matrix", WINDOW_AUTOSIZE);
	imshow("Landscape", img);
	Size sz = img.size();
	int height = sz.height;
	int width = sz.width;
	int ca = cvWaitKey(0);
	// Esc = 27, a = 97, d = 100, s = 115, w =119
	while (ca != 27) {
		// Looping till get the command 'a', 'd', 'w', 's'
		while (ca != 97 && ca != 100 && ca != 115 && ca != 119 && ca != 27) {
			ca = cvWaitKey(0);
		}
		if (width <= 1 || height <= 1) break;
		//Best Seam Matrix 1xheight (contain the row x column that is the best seam)
		vector<int> BestSeam;
		// Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal bestseam
		// 'a' => Reduce width, d => increase width
		if (ca == 97 || ca == 100) {
			// Construct M matrix and K matrix in the vertical direction
			// *** WRITE YOUR CODE ***
			Mat imgPadded;
			Mat grayimg;
			cvtColor(img, grayimg, COLOR_BGR2GRAY);
			Mat M(Size(width, height), CV_8UC1, Scalar(0));
			Mat K(Size(width, height), CV_8UC1, Scalar(0));
			int border = 1;
		copyMakeBorder(grayimg, imgPadded, border, border, border, border, BORDER_REPLICATE);
		for (int i = 1; i < imgPadded.rows - 1; i++) { //Starting from 1 to rows-1; Because of padded(instead of matrix(-1,-1))
			for (int j = 1; j < imgPadded.cols - 1; j++) {
				int r = i - 1; //i and j are the coordinated in imgPadded(one more pixel at border)
				int co = j - 1; //r and c are the coordinated of M matrix and original image(unchanged resolution)
				int cL = abs((int)imgPadded.at<uchar>(i, j + 1) - (int)imgPadded.at<uchar>(i, j - 1)) + abs((int)imgPadded.at<uchar>(i - 1, j) - (int)imgPadded.at<uchar>(i, j - 1));
				int cU = abs((int)imgPadded.at<uchar>(i, j + 1) - (int)imgPadded.at<uchar>(i, j - 1));
				int cR = abs((int)imgPadded.at<uchar>(i, j + 1) - (int)imgPadded.at<uchar>(i, j - 1)) + abs((int)imgPadded.at<uchar>(i - 1, j) - (int)imgPadded.at<uchar>(i, j + 1));
				int topLeft = ((co == 0 || r == 0) ? cL : ((int)M.at<uchar>(r - 1, co - 1) + cL));
				int topUp = ((r == 0) ? cU : ((int)M.at<uchar>(r - 1, co) + cU));
				int topRight = ((co >= width - 1 || r == 0) ? cR : ((int)M.at<uchar>(r - 1, co + 1) + cR));
				M.at<uchar>(r, co) = min(topLeft, min(topUp, topRight));
				if (r == 0) K.at<uchar>(r, co) = 0;
				else {
					if (co!=0 && topLeft == min(topLeft, min(topUp, topRight))) {
						K.at<uchar>(r, co) = 1;
					}
					else if (topUp == min(topLeft, min(topUp, topRight))) {
						K.at<uchar>(r, co) = 2;
					}
					else {
						if(co>=width-1)
							K.at<uchar>(r, co) = 2;
						else {
							K.at<uchar>(r, co) = 3;
						}
					}
				}
			}
		}
		//cvWaitKey(1);
		//imshow("M matrix", M);
		// Find the best seam in the veritical direction
		// *** WRITE YOUR CODE ***
		Mat B(Size(width, height), CV_8UC1, Scalar(0));
		int row = 0;
		for (int i = 0; i < width; i++) {
			if ((int)M.at<uchar>(height - 1, i) < (int)M.at<uchar>(height - 1, row)) row = i;
		}
		int r = height - 1;
		B.at<uchar>(r, row) = 255;
		BestSeam.push_back(row);
		while (r > 0) {
			if ((int)K.at<uchar>(r, row) == 1) {
				B.at<uchar>(r - 1, row - 1) = 255;
				BestSeam.push_back(row - 1);
				row--;
			}
			else if ((int)K.at<uchar>(r, row) == 2) {
				B.at<uchar>(r - 1, row) = 255;
				BestSeam.push_back(row);
			}
			else if ((int)K.at<uchar>(r, row) == 3) {
				B.at<uchar>(r - 1, row + 1) = 255;
				BestSeam.push_back(row + 1);
				row++;
			}
			r--;
		}
		reverse(BestSeam.begin(), BestSeam.end());
		//cvWaitKey(1);
		//imshow("Best Seam", B);
		//copy(BestSeam.begin(), BestSeam.end(), ostream_iterator<int>(cout, " "));
		grayimg.release();
		imgPadded.release();
		M.release();
		K.release();
		B.release();
		}

		// 'w' => increase height, 's' => reduce height
		if (ca == 115 || ca == 119) {
			// Construct M matrix and K matrix in the horizontal direction
			// *** WRITE YOUR CODE ***
			Mat imgPadded;
			Mat grayimg;
			cvtColor(img, grayimg, COLOR_BGR2GRAY);
			Mat M(Size(width, height), CV_8UC1);
			M = 0;
			Mat K(Size(width, height), CV_8UC1);
			K = 0;
			int border = 1;
			copyMakeBorder(grayimg, imgPadded, border, border, border, border, BORDER_REPLICATE);
			for (int j = 1; j < imgPadded.cols - 1; j++) { //Starting from 1 to rows-1; Because of padded(instead of matrix(-1,-1))
				for (int i = 1; i < imgPadded.rows - 1; i++) {
					int r = i - 1; //i and j are the coordinated in imgPadded(one more pixel at border)
					int c = j - 1; //r and c are the coordinated of M matrix and original image(unchanged resolution)
					int cL = abs((int)imgPadded.at<uchar>(i, j-1) - (int)imgPadded.at<uchar>(i+1, j)) + abs((int)imgPadded.at<uchar>(i-1, j) - (int)imgPadded.at<uchar>(i+1, j));
					int cU = abs((int)imgPadded.at<uchar>(i-1, j) - (int)imgPadded.at<uchar>(i+1, j));
					int cR = abs((int)imgPadded.at<uchar>(i, j-1) - (int)imgPadded.at<uchar>(i-1, j)) + abs((int)imgPadded.at<uchar>(i-1, j) - (int)imgPadded.at<uchar>(i+1, j));
					int topLeft = (c == 0 || r >= height-1) ? cL : (int)M.at<uchar>(r + 1, c - 1) + cL;
					int topUp = (c == 0) ? cU : (int)M.at<uchar>(r, c - 1) + cU;
					int topRight = (c == 0 || r == 0) ? cR : (int)M.at<uchar>(r - 1, c - 1) + cR;
					M.at<uchar>(r, c) = min(topLeft, min(topUp, topRight));
					if (c == 0) K.at<uchar>(r, c) = 0;
					else {
						if (r != height-1 && topLeft == min(topLeft, min(topUp, topRight))) {
							K.at<uchar>(r, c) = 1;
						}
						else if (topUp == min(topLeft, min(topUp, topRight))) {
							K.at<uchar>(r, c) = 2;
						}
						else {
							if (r == 0)
								K.at<uchar>(r, c) = 2;
							else {
								K.at<uchar>(r, c) = 3;
							}
						}
					}
				}
			}
			//cvWaitKey(1);
			//imshow("M matrix", M);
			// Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***
			Mat B(Size(width, height), CV_8UC1);
			B = 0;
			int col = 0;
			for (int j = 0; j < height; j++) {
				if ((int)M.at<uchar>(j, width - 1) < (int)M.at<uchar>(col, width - 1)) col = j;
			}
			int c = width - 1;
			B.at<uchar>(col, c) = 255;
			BestSeam.push_back(col);
			while (c > 0) {
				if ((int)K.at<uchar>(col, c) == 1) {
					B.at<uchar>(col + 1, c - 1) = 255;
					BestSeam.push_back(col + 1);
					col++;
				}
				else if ((int)K.at<uchar>(col, c) == 2) {
					B.at<uchar>(col, c - 1) = 255;
					BestSeam.push_back(col);
				}
				else if ((int)K.at<uchar>(col, c) == 3) {
					B.at<uchar>(col - 1, c - 1) = 255;
					BestSeam.push_back(col - 1);
					col--;
				}
				c--;
			}
			reverse(BestSeam.begin(), BestSeam.end());
			//cvWaitKey(1);
			//imshow("Best Seam", B);
			grayimg.release();
			imgPadded.release();
			M.release();
			K.release();
			B.release();
		}

		// Insert or delete the best seam
		if (ca == 97) {
			// Reduce width or delete seam vertically
			// Copy the pixels into this image
			Mat img_new(height, --width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					for (int k = 0; k < img.channels(); k++) {
						if (j > BestSeam[i]) {
							img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j + 1)[k];
						}
						else img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j)[k];
					}
				}
			}
			// Show resized image
			cvWaitKey(1);
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
			BestSeam.clear();
		}
		if (ca == 100) {
			// Increase width or insert seam vertically
			// Copy the pixels into this image
			Mat img_new(height, ++width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					for (int k = 0; k < img.channels(); k++) {
						if (j > BestSeam[i]) {
							img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j - 1)[k];
						}
						else img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j)[k];
					}
				}
			}
			// Show resized image
			cvWaitKey(1);
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
			BestSeam.clear();
		}
		if (ca == 115) {
			// Reduce height or delete seam horizontally
			// Copy the pixels into this image
			Mat img_new(--height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			for (int j = 0; j < width; j++) {
				for (int i = 0; i < height; i++) {
					for (int k = 0; k < img.channels(); k++) {
						if (i > BestSeam[j]) {
							img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i + 1, j)[k];
						}
						else img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j)[k];
					}
				}
			}
			// Show resized image
			cvWaitKey(1);
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
			BestSeam.clear();
		}
		if (ca == 119) {
			// Increase height or insert seam horizontally
			// Copy the pixels into this image
			Mat img_new(++height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			for (int j = 0; j < width; j++) {
				for (int i = 0; i < height; i++) {
					for (int k = 0; k < img.channels(); k++) {
						if (i > BestSeam[j]) {
							img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i-1, j)[k];
						}
						else img_new.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j)[k];
					}
				}
			}
			// Show resized image
			cvWaitKey(1);
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
			BestSeam.clear();
		}
		if (ca == 27) {
			break;
			ca = cvWaitKey(0);
		}
	}
	return 0;
}