//#include <stdafx.h>  
#include <iostream> 
using namespace std;
#include <openCV/cv.h>  
#include <openCV/cxcore.h>  
#include <openCV/highgui.h>  

using namespace cv;

char * imgname[] = 
{ 
"F:\\1.jpg",
"F:\\2.jpg",
/*
"F:\\计算机视觉项目\\Camera Roll\\3.jpg",
"F:\\计算机视觉项目\\Camera Roll\\4.jpg",
"F:\\计算机视觉项目\\Camera Roll\\5.jpg",
"F:\\计算机视觉项目\\Camera Roll\\6.jpg",
"F:\\计算机视觉项目\\Camera Roll\\7.jpg",
"F:\\计算机视觉项目\\Camera Roll\\8.jpg",
"F:\\计算机视觉项目\\Camera Roll\\9.jpg",
"F:\\计算机视觉项目\\Camera Roll\\10.jpg",
"F:\\计算机视觉项目\\Camera Roll\\11.jpg",
"F:\\计算机视觉项目\\Camera Roll\\12.jpg",
"F:\\计算机视觉项目\\Camera Roll\\13.jpg",
"F:\\计算机视觉项目\\Camera Roll\\14.jpg",
"F:\\计算机视觉项目\\Camera Roll\\15.jpg",
"F:\\计算机视觉项目\\Camera Roll\\16.jpg",
"F:\\计算机视觉项目\\Camera Roll\\17.jpg"
*/
};

// 计算图像直方图  
void CompImageHist(Mat &src, MatND &b_hist, MatND &g_hist, MatND &r_hist)
{
	// 分割成3个单通道图像(bgr)  
	vector<Mat> rgb_planes;
	split(src, rgb_planes);

	// 设定bin数目及取值范围  
	int histSize = 255;
	float range[] = { 0, 255 };
	const float* histRange = { range };

	// 计算直方图  
	bool uniform = true;
	bool accumulate = false;
	calcHist(&rgb_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// 直方图归一化>>范围为[0, 1]  
	normalize(r_hist, r_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
	normalize(g_hist, g_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
	normalize(b_hist, b_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
}

double get_avg_gray(IplImage *img)
{
	IplImage *gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img, gray, CV_RGB2GRAY);
	CvScalar scalar = cvAvg(gray);
	cvReleaseImage(&gray);
	return scalar.val[0];
}

double Match(char* imgname1,char* imgname2)
{
	 


	//char * imgname1 ="F:\\计算机视觉项目\\Camera Roll\\1.jpg";

	// 得到两张图的亮度值
	IplImage * Iplimg1 = cvLoadImage(imgname1);
	IplImage * Iplimg2 = cvLoadImage(imgname2);
	double avg_gray1 = get_avg_gray(Iplimg1);
	double avg_gray2 = get_avg_gray(Iplimg2);
	//cout<<avg_gray1<<"***"<<avg_gray2;

	Mat imgp0 = imread(imgname1);
	// imshow("img0", imgp0);  
	Mat imgp1 = imread(imgname2);
	// imshow("img0", imgp0);   
	Mat img0;
	Mat img1;
	//     调节图像的亮度，使两图像尽可能亮度相似
	imgp0.convertTo(img0, -1, 1.0, 200 - avg_gray1);
	imgp1.convertTo(img1, -1, 1.0, 200 - avg_gray2);
	//imshow("img0", img0); 
	//imshow("img1", img1); 


	//waitKey();

	MatND hist0[3], hist1[3];
	// 计算图像直方图  
	CompImageHist(img0, hist0[0], hist0[1], hist0[2]);
	CompImageHist(img1, hist1[0], hist1[1], hist1[2]);

	double sum[4] = { 0.0 };
	double results[4] = { 0.0 };
	char channelName[][8] = { { "蓝色" }, { "绿色" }, { "红色" } };

	// 比较直方图  
	for (int i = 0; i < 3; i++)
	{
		// 相关: CV_COMP_CORREL,卡方: CV_COMP_CHISQR,相交: CV_COMP_INTERSECT,巴氏: CV_COMP_BHATTACHARYYA  
		results[0] = compareHist(hist0[i], hist1[i], CV_COMP_CORREL);
		results[1] = compareHist(hist0[i], hist1[i], CV_COMP_CHISQR);
		results[2] = compareHist(hist0[i], hist1[i], CV_COMP_INTERSECT);
		results[3] = compareHist(hist0[i], hist1[i], CV_COMP_BHATTACHARYYA);
		sum[0] += results[0];
		sum[1] += results[1];
		sum[2] += results[2];
		sum[3] += results[3];
	}
	return sum[0]/3;
}

char* picMatch(char* imgname1)
{
	double Max=0.0,s=0.0;
	char* img;
	int j;
	for (int i = 0; i < 2; i++)
	{
		img = imgname[i];
		s = Match(imgname1, img);
		if (Max < s)
		{
			Max = s;
			j = i;
		}
	}
	return imgname[j];
}