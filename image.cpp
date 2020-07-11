#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>  
#include <sstream>
#include "image.h"

using namespace cv;
using namespace std;


Mat IMAGE::get_im(void) {
	Mat image_work = imread(dir_im, 1); // Open original image
    vector<Mat> rgb(3);
    split(image_work,rgb);  // Dividing the image in RGB   
    Mat image_gray = rgb[1];  // Choose the green channel
	float top = (int) (pad); float bottom = (int) (pad);
	float left = (int) (pad); float right = (int) (pad);
	imW = image_gray.clone();
	cv::copyMakeBorder( image_gray, imW, top, bottom, 
			left, right, BORDER_CONSTANT, 0 );   // Making the border padding
	return imW;
}

Mat IMAGE::get_mask_fov(void) {
	Mat un_mask_FOV = imread(dir_mask_FOV, CV_LOAD_IMAGE_GRAYSCALE);
	float top = (int) (pad); float bottom = (int) (pad);
	float left = (int) (pad); float right = (int) (pad);
	mask_FOV = un_mask_FOV.clone();
	cv::copyMakeBorder( un_mask_FOV, mask_FOV, top, bottom, 
			left, right, BORDER_CONSTANT, 0 );   // Making the border padding
	return mask_FOV;
}

Mat IMAGE::get_mask_v(void) {
	Mat un_mask_V = imread(dir_mask_v, CV_LOAD_IMAGE_GRAYSCALE);
	float top = (int) (pad); float bottom = (int) (pad);
	float left = (int) (pad); float right = (int) (pad);
	mask_V = un_mask_V.clone();
	cv::copyMakeBorder( un_mask_V, mask_V, top, bottom, 
			left, right, BORDER_CONSTANT, 0 );   // Making the border padding
	return mask_V;
}

int IMAGE::get_pad(void) const {
	return pad;
}