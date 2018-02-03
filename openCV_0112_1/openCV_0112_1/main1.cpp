

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


#define  EXPORTBUILD   

using namespace std;
using namespace cv;


#if defined (EXPORTBUILD)    
# define _DLLExport __declspec (dllexport)    
# else    
# define _DLLExport __declspec (dllimport)    
#endif    


//定义函数指针;  
typedef void(__stdcall *CPPCallback)(int xPos,int yPos);


//extern "C"  int _DLLExport AddNumber(int x, int y);
extern "C"  void _DLLExport showUnity3dCamera(char* imageData, int width, int height, int stride, CPPCallback callback);




//extern "C" void _DLLExport SetCallback(CPPCallback callback);


#pragma comment(lib, "opencv_world320d.lib")

//void SetCallback(CPPCallback callback)
//{
//	int tick = 1223;
//	callback(tick);
//}



CPPCallback cp;

String face_cascade_name = "haarcascade_frontalface_default.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;

String window_name = "Capture - Face detection";




void detectAndDisplay(Mat frame, CPPCallback callback)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
	
	for (size_t i = 0; i < faces.size()&&i<1; i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		callback(center.x,center.y);
	}
	//-- Show what you got
	
	//imshow(window_name, frame);

	
}


void showUnity3dCamera(char* imageData, int width, int height, int stride, CPPCallback callback)
{

	if (NULL == imageData)
	{
		return;
	}
	cv::Mat opencvImage(height, width, CV_8UC4);
	memcpy(opencvImage.data, imageData, stride*height*width);
	//cv::cvtColor(opencvImage, opencvImage, CV_BGR2RGB);
	Mat ms;
	ms.create(opencvImage.size(), CV_8UC4);
	for (int i = 0; i < opencvImage.rows; i++) {
		for (int j = 0; j < opencvImage.cols; j++) {
			//Vec
			ms.at<Vec4b>(i,j)[0]=opencvImage.at<Vec4b>(opencvImage.rows-1-i,j)[2];
			ms.at<Vec4b>(i, j)[1] = opencvImage.at<Vec4b>(opencvImage.rows - 1 - i, j)[1];
			ms.at<Vec4b>(i, j)[2] = opencvImage.at<Vec4b>(opencvImage.rows - 1 - i, j)[0];
		}
	}
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return ; };
	detectAndDisplay(ms,callback);
    //cv::imshow("11",ms);  
	//cv::waitKey(1);  
   
}


int main2() {

	VideoCapture capture;
	Mat frame;
	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	//if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading eyes cascade\n"); return -1; };
	//-- 2. Read the video stream
	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame,NULL);
		char c = (char)waitKey(10);
		if (c == 27) { break; } // escape
	}
	return 0;
	

}


