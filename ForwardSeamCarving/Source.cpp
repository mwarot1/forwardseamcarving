#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

//Forward Seam Carving
int main() {
	Mat img = imread("LS.jpg", IMREAD_COLOR);
	namedWindow("Landscape", WINDOW_AUTOSIZE);
	imshow("Landscape", img);
	int c = cvWaitKey(0);
	Size sz = img.size();
	int height = sz.height;
	int width = sz.width;
	
	// Esc = 27, a = 97, d = 100, s = 115, w =119
	while (c != 27) {
		// Looping till get the command 'a', 'd', 'w', 's'
		while (c != 97 && c != 100 && c != 115 && c != 119 && c != 27) {
			c = cvWaitKey(0);
		}

		// Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal bestseam
		// 'a' => Reduce width, d => increase width
		if (c == 97 || c == 100) {
			// Construct M matrix and K matrix in the vertical direction
			// *** WRITE YOUR CODE ***

			// Find the best seam in the veritical direction
			// *** WRITE YOUR CODE ***
		}

		// 'w' => increase height, 's' => reduce height
		if (c == 115 || c == 119) {
			// Construct M matrix and K matrix in the horizontal direction
			// *** WRITE YOUR CODE ***

			// Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***
		}

		// Insert or delete the best seam
		if (c == 97) {
			// Reduce width or delete seam vertically
			// Copy the pixels into this image
			Mat img_new(height, --width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			// Show resized image
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 100) {
			// Increase width or insert seam vertically
			// Copy the pixels into this image
			Mat img_new(height, ++width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			// Show resized image
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 115) {
			// Reduce height or delete seam horizontally
			// Copy the pixels into this image
			Mat img_new(--height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			// Show resized image
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 119) {
			// Increase height or insert seam horizontally
			// Copy the pixels into this image
			Mat img_new(++height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			// Show resized image
			imshow("Landscape", img_new);
			// Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 27) {
			break;
			c = cvWaitKey(0);
		}
		return 0;
	}
}