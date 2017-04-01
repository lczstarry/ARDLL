// cvCalib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int Nimages, float SquareSize);

int image_width = 768;
int image_height = 576;//待标定图片的大小
const int ChessBoardSize_w = 7;
const int ChessBoardSize_h = 7;//图片中可标定的角数
const CvSize  ChessBoardSize = cvSize(ChessBoardSize_w,ChessBoardSize_h);
const int NPoints = ChessBoardSize_w*ChessBoardSize_h;//角点个数
const int NImages=6;//待标定的图片数
int corner_count[NImages] = {0};
float    SquareWidth = 10; //棋盘格子的边长，可任意设定，不影响内参数

CvMat *intrinsics;
CvMat *distortion_coeff;
CvMat *rotation_vectors;
CvMat *translation_vectors;
CvMat *object_points;
CvMat *point_counts;
CvMat *image_points;

void main()
{
	IplImage     *current_frame_rgb; 
	IplImage     *current_frame_gray;
	IplImage     *chessBoard_Img;
	CvPoint2D32f corners[NPoints*NImages];
	
	chessBoard_Img =cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
	current_frame_gray = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
	current_frame_rgb = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
	
	int captured_frames=0;
	for(captured_frames=0;captured_frames<NImages;captured_frames++)
	{
		char filename[]="cal20Img00.bmp";   //说明：我把待标定的图片的名子依次命名为：01.jpg, 02.jpg, 03.jpg, 04.jpg,……
		if(captured_frames<9)
			filename[9]=(char)(captured_frames+49);
		else if(captured_frames>=9&&captured_frames<=99)
		{
			int j,jj;
			jj=(captured_frames+1)/10;
			j=(captured_frames+1)%10;
			filename[8]=jj+48;
			filename[9]=j+48;
		} 
		else 
			printf("error, too many images.......\n"); //load images end
		
		chessBoard_Img=cvLoadImage( filename, CV_LOAD_IMAGE_COLOR );
		cvCvtColor(chessBoard_Img, current_frame_gray, CV_BGR2GRAY);
        cvCopy(chessBoard_Img,current_frame_rgb);
		
		int find_corners_result;
		find_corners_result = cvFindChessboardCorners(current_frame_gray,
			ChessBoardSize,
			&corners[captured_frames*NPoints],
			&corner_count[captured_frames],//int corner_count[NImages] = {0};
			CV_CALIB_CB_ADAPTIVE_THRESH );
		cvFindCornerSubPix( current_frame_gray, 
			&corners[captured_frames*NPoints],
			NPoints, cvSize(2,2),cvSize(-1,-1), 
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03) );
		cvDrawChessboardCorners(current_frame_rgb, ChessBoardSize, //绘制检测到的棋盘角点
			&corners[captured_frames*NPoints], 
			NPoints, 
			find_corners_result);


		cvNamedWindow("Window 0", 1);
		cvNamedWindow("result", 1); 
		cvShowImage("Window 0",chessBoard_Img);
		cvShowImage("result",current_frame_rgb);
		cvWaitKey(0);
	}
	
	intrinsics         = cvCreateMat(3,3,CV_32FC1);
	distortion_coeff     = cvCreateMat(1,4,CV_32FC1);
	rotation_vectors     = cvCreateMat(NImages,3,CV_32FC1);
	translation_vectors     = cvCreateMat(NImages,3,CV_32FC1);
	point_counts         = cvCreateMat(NImages,1,CV_32SC1);
	object_points     = cvCreateMat(NImages*NPoints,3,CV_32FC1);
	image_points         = cvCreateMat(NImages*NPoints,2,CV_32FC1);
	
	//把2维点转化成三维点（object_points输出量），

	InitCorners3D(object_points, ChessBoardSize, NImages, SquareWidth);
	cvSetData( image_points, corners, sizeof(CvPoint2D32f));
	cvSetData( point_counts, &corner_count, sizeof(int));
//	float test_object_points[20000];
//		float test_image_point[10000];
//		float test_point_counts[4000];
//		for (int z=0;z<NPoints*NImages;z++)
//		{
//			test_object_points[z*3+0]=cvmGet(object_points,z,0);
//			test_object_points[z*3+1]=cvmGet(object_points,z,1);
//			test_object_points[z*3+2]=cvmGet(object_points,z,2);
//
//			test_image_point[z*2+0]=cvmGet(image_points,z,0);
//			test_image_point[z*2+1]=cvmGet(image_points,z,1);
//			
//			test_point_counts[z]=cvmGet(point_counts,z,0);
//
//		}
	//计算内参
	cvCalibrateCamera2( object_points,
		image_points,
		point_counts,
		cvSize(image_width,image_height),
		intrinsics,
		distortion_coeff,
		rotation_vectors,
		translation_vectors,
		0);
	
	float intr[3][3] = {0.0};
	float dist[4] = {0.0};
	float tranv[3] = {0.0};
	float rotv[3] = {0.0};

	
	for ( int i = 0; i < 3; i++)
	{
		for ( int j = 0; j < 3; j++)
		{
			intr[i][j] = ((float*)(intrinsics->data.ptr + intrinsics->step*i))[j];
		}
		dist[i] = ((float*)(distortion_coeff->data.ptr))[i];
		tranv[i] = ((float*)(translation_vectors->data.ptr))[i];
		rotv[i] = ((float*)(rotation_vectors->data.ptr))[i];
	}
	dist[3] = ((float*)(distortion_coeff->data.ptr))[3];
	
	//以上部分不明白

	printf("-----------------------------------------\n ");
	printf("INTRINSIC MATRIX:  \n");
	printf("[ %6.4f %6.4f %6.4f ]  \n", intr[0][0], intr[0][1], intr[0][2]);
	printf("[ %6.4f %6.4f %6.4f ]  \n", intr[1][0], intr[1][1], intr[1][2]);
	printf("[ %6.4f %6.4f %6.4f ]  \n", intr[2][0], intr[2][1], intr[2][2]);
	printf("----------------------------------------- \n");
	printf("DISTORTION VECTOR:  \n");
	printf("[ %6.4f %6.4f %6.4f %6.4f ]  \n", dist[0], dist[1], dist[2], dist[3]);
	printf("----------------------------------------- \n");
	printf("ROTATION VECTOR:  \n");
	printf("[ %6.4f %6.4f %6.4f ]  \n", rotv[0], rotv[1], rotv[2]);
	printf("TRANSLATION VECTOR:  \n");
	printf("[ %6.4f %6.4f %6.4f ]  \n", tranv[0], tranv[1], tranv[2]);
	printf("----------------------------------------- \n");
	
	cvReleaseMat(&intrinsics);       
	cvReleaseMat(&distortion_coeff); 
	cvReleaseMat(&rotation_vectors);
	cvReleaseMat(&translation_vectors);   
	cvReleaseMat(&point_counts);
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvDestroyAllWindows();
}

void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize)
{
	int CurrentImage = 0;
	int CurrentRow = 0;
	int CurrentColumn = 0;
	int NPoints = ChessBoardSize.height*ChessBoardSize.width;
	float * temppoints = new float[NImages*NPoints*3];
	
	// for now, assuming we're row-scanning
	for (CurrentImage = 0 ; CurrentImage < NImages ; CurrentImage++)
	{
		for (CurrentRow = 0; CurrentRow < ChessBoardSize.height; CurrentRow++)
		{
			for (CurrentColumn = 0; CurrentColumn < ChessBoardSize.width; CurrentColumn++)
			{
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3]=(float)CurrentRow*SquareSize;
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3+1]=(float)CurrentColumn*SquareSize;
				temppoints[(CurrentImage*NPoints*3)+(CurrentRow*ChessBoardSize.width + 
					CurrentColumn)*3+2]=0.f;
			}
		}
	}
	(*Corners3D) = cvMat(NImages*NPoints,3,CV_32FC1, temppoints);
}
