### 11-1
#### //11-1-1
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat calcGrayHist(const Mat& src);
Mat getGrayHistImage(const Mat& hist);
//void onMouse(int event, int x, int y, int flags, void* userdata);
int main(void) {
	Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);
	Mat hist = calcGrayHist(src);
	Mat imgHist = getGrayHistImage(hist);

	Mat dst;
	threshold(src, dst, 93, 255, THRESH_BINARY);

	imshow("src", src);
	imshow("srcHist", imgHist);
	imshow("dst", dst);

	//setMouseCallback("srcHist", onMouse, &imgHist);
	waitKey();
	return 0;
}
Mat calcGrayHist(const Mat& src) {
	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	int histSize[] = { 256 };
	float graylevel[] = { 0,256 };
	const float* ranges[] = { graylevel };

	calcHist(&src, 1, channels, Mat(), hist, dims, histSize, ranges);
	return hist;
}
Mat getGrayHistImage(const Mat& hist) {
	double histMax;
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8UC1, Scalar(255));
	for (int i = 0; i < 256; i++) {
		line(imgHist, Point(i, 100), Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
	}
	return imgHist;
}
//void onMouse(int event, int x, int y, int flags, void* userdata) {
//	Mat img = *(Mat*)userdata;
//	if (event == EVENT_LBUTTONDOWN)
//		cout << x << endl;
//}


#### //11-1-2
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat calcGrayHist(const Mat& src);
Mat getGrayHistImage(const Mat& hist);
int main(void) {
	Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);
	Mat hist = calcGrayHist(src);
	Mat imgHist = getGrayHistImage(hist);

	Mat dst;
	double t = threshold(src, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);
	cout << t << endl;

	imshow("src", src);
	imshow("srcHist", imgHist);
	imshow("dst", dst);

	waitKey();
	return 0;
}
Mat calcGrayHist(const Mat& src) {
	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	int histSize[] = { 256 };
	float graylevel[] = { 0,256 };
	const float* ranges[] = { graylevel };

	calcHist(&src, 1, channels, Mat(), hist, dims, histSize, ranges);
	return hist;
}
Mat getGrayHistImage(const Mat& hist) {
	double histMax;
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8UC1, Scalar(255));
	for (int i = 0; i < 256; i++) {
		line(imgHist, Point(i, 100), Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
	}
	return imgHist;
}*/



#### //11-1-3
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void bSize_onChange(int pos, void* userdata);
void C_onChange(int pos, void* userdata);
int main(void) {
	Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);
	namedWindow("Trackbar");
	createTrackbar("block_Size", "Trackbar", 0, 2000, bSize_onChange, &src);
	createTrackbar("C_Size", "Trackbar", 0, 100, bSize_onChange, &src);

	imshow("src", src);
	waitKey();
	return 0;
}
void bSize_onChange(int pos, void* userdata) {
	Mat src = *(Mat*)userdata;
	Mat dst;
	int bsize = pos;
	if (bsize % 2 == 0) bsize--;
	if (bsize < 3) bsize = 3;
	int c = getTrackbarPos("C_Size", "Trackbar");
	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, c);
	imshow("dst", dst);
}
void C_onChange(int pos, void* userdata) {
	Mat src = *(Mat*)userdata;
	Mat dst;
	int bsize = getTrackbarPos("block_Size", "Trackbar");
	if (bsize % 2 == 0) bsize--;
	if (bsize < 3) bsize = 3;
	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, pos);
	imshow("dst", dst);
}



#### //11-1-4
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat calcGrayHist(const Mat& src);
Mat getGrayHistImage(const Mat& hist);
int main(void) {
	VideoCapture cap("6_lt_ccw_100rpm_out.mp4");
	double fps = cap.get(CAP_PROP_FPS);
	int delay = cvRound(1000 / fps);
	Mat frame;
	while (true) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		Mat hist = calcGrayHist(frame);
		Mat imgHist = getGrayHistImage(hist);

		Mat dst;
		frame.copyTo(dst);

		threshold(frame, dst, 150, 255, THRESH_BINARY);
		//adaptiveThreshold(frame, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 249, 0);
		dst(Rect(0, 0, frame.cols, frame.rows * 3 / 5)) = 0;
		dst(Rect(frame.cols * 2 / 3, frame.rows * 3 / 5, frame.cols - frame.cols * 2 / 3, frame.rows - frame.rows * 3 / 5)) = 0;

		imshow("src", frame);
		imshow("srcHist", imgHist);
		imshow("dst", dst);
		waitKey(delay);
	}
	return 0;
}
Mat calcGrayHist(const Mat& src) {
	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	int histSize[] = { 256 };
	float graylevel[] = { 0,256 };
	const float* ranges[] = { graylevel };

	calcHist(&src, 1, channels, Mat(), hist, dims, histSize, ranges);
	return hist;
}
Mat getGrayHistImage(const Mat& hist) {
	double histMax;
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8UC1, Scalar(255));
	for (int i = 0; i < 256; i++) {
		line(imgHist, Point(i, 100), Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
	}
	return imgHist;
}



### 11-2
#### //11-2-1
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(void) {
	Mat m1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat m2 = getStructuringElement(MORPH_CROSS, Size(3, 3));
	//Mat m3 = getStructuringElement(MORPH_CROSS, Size(5, 3));
	Mat m3 = getStructuringElement(MORPH_ELLIPSE, Size(5, 3));
	cout << "MORPH_RECT:" << endl << m1 << endl;
	cout << "MORPH_CROSS:" << endl << m2 << endl;
	cout << "MORPH_ELLIPSE:" << endl << m3 << endl;
	return 0;
}



#### //11-2-2
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(void) {
	Mat src = imread("letterj.png", IMREAD_GRAYSCALE);
	Mat bin;
	double t = threshold(src, bin, 140, 255, THRESH_BINARY);
	Mat dst;
	morphologyEx(bin, dst, MORPH_CLOSE, Mat());
	imshow("src", src);
	imshow("bin", bin);
	imshow("erode", dst);
	waitKey();
	return 0;
}



#### //11-2-3
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(void) {
	Mat src = imread("letterj2.png", IMREAD_GRAYSCALE);
	Mat bin;
	threshold(src, bin, 200, 255, THRESH_BINARY);
	Mat edge;
	morphologyEx(bin, edge, MORPH_GRADIENT, Mat()); //팽창-침식
	imshow("src", src);
	imshow("bin", bin);
	imshow("edge", edge);
	waitKey();
	return 0;
}



#### //11-2-4
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(void) {
	Mat src = imread("car.png");
	Mat dst;
	cvtColor(src, dst, COLOR_BGR2GRAY);
	Mat dst_blur;
	blur(dst, dst_blur, Size(5, 5));
	Mat dx;
	Sobel(dst_blur, dx, -1, 1, 0);
	Mat binary;
	threshold(dx, binary, 110, 255, THRESH_BINARY);
	Mat final;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(40, 5));
	morphologyEx(binary, final, MORPH_ERODE, Mat());
	morphologyEx(final, final, MORPH_CLOSE, kernel);
	imshow("image", src);
	imshow("x축 방향 소벨", dx);
	imshow("이진화 영상", binary);
	imshow("닫힘 영상", final);
	waitKey();
	return 0;
}
