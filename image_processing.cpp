#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>  
#include <sstream>
#include "image_processing.h"


using namespace cv;
using namespace std;

Mat IMAGE_PROCESSING::get_kernel_mean(void) {
	Mat kernel = (Mat::ones(dimension_mean, dimension_mean, CV_32F)) * 1/25;
	return kernel;
}


void IMAGE_PROCESSING::print_mean(void) {
	correlation_mean();
	string name = dir_out + string("Pre-processed image ") 
		+ number + string(".png");
	imwrite(name, final_mean);
}



void IMAGE_PROCESSING::print_kernels(void) {
	Mat kernel_med = (Mat::zeros(dimension_match, dimension_match, CV_32F));
	string out = dir_out + string("kernels") + number + string(".txt");
	ofstream outfile (out);
	for (angle = 0; angle <= 165 ; angle = angle + 15){
		kernel_med = kernel_fabrication();
		outfile << angle / 15 << ": " << endl << " " << kernel_med << endl << endl;
	}
	outfile.close();
}


void IMAGE_PROCESSING::print_kernel(int theta) {
	angle = theta;
	Mat kernel = kernel_fabrication();
	cout << theta << ": " << endl << " " << kernel << endl << endl;
}

void IMAGE_PROCESSING::print_image_matched (void) {
	correlation_cycle();
	string name = dir_out + string("Matched Filter Result ") 
		+ number + string(".png");
	imwrite(name, final_image_matched);
}

void IMAGE_PROCESSING::print_image_final (void) {
	make_threshold();
	make_mult_mask();
	string name = dir_out + string("Pos-processed image ") 
		+ number + string(".png");
	imwrite(name, final_image);
}

Mat IMAGE_PROCESSING::get_image_final (void) const {
	return final_image;
}


void IMAGE_PROCESSING::correlation_mean() {
	int mean = dimension_mean * dimension_mean;				  
    int lin = (size_t)image_o.size().height;
    int cols = (size_t)image_o.size().width;   
    final_mean = (Mat::zeros(lin,cols, CV_32F));
    Mat kernel = Mat::ones(dimension_mean, dimension_mean, CV_32F);
    // Runs all the image, pixel by pixel, centering the kernel in the pixel.
    // It multiplies the repective kernel value for the correspondent image pixel,
    // suming all the values and dividing by dim*dim, where dim reprensents the dimension.
    // of the kernel. The respective pixel is substitute by the resultant value
    for (int row = pad; row < lin - pad; row++){ 
        for (int col = pad; col < cols - pad; col++){
            float count = 0; 
            // Run all the image pixeis.  In each pixel, centers the kernel, and multiply
            // the value of the kernel for the image corresponding.
            for (int i = - dimension_mean / 2; i < dimension_mean / 2 + 1; i++){
                for (int j = - dimension_mean / 2; j < dimension_mean / 2 + 1; j++){                    
                    count = count + image_o.at<uchar>(row + i,col + j) 
                    	* kernel.at<float>(i + dimension_mean / 2,j 
                    		+ dimension_mean / 2);  
                }

            	final_mean.at<float>(row,col) = float (count/mean);
            }
        }
    }
}


void IMAGE_PROCESSING::correlation_cycle() {
	//Mat intermed_image = (Mat::zeros(final_mean.size(), CV_32F)) - 1e12;
	final_image_matched = (Mat::zeros(final_mean.size(), CV_32F)) - 1e12;

	for (angle = 0; angle <= 165 ; angle += 15){  
		kernel = kernel_fabrication();   // Creates kernel
		Mat image_matched = correlation_matched();
		//Picks the maximum values from the correlated image
		final_image_matched = maximos_kernel(image_matched, final_image_matched); 
	}
}


	
Mat IMAGE_PROCESSING::kernel_fabrication() {
	Mat kernel = (Mat::zeros(dimension_match, dimension_match, CV_32F));
    float sigmasq = 2.0 * sigma * sigma;
    float angleR = angle * M_PI / 180.0f, cosR = cosf(angleR), 
    	sinR = sinf(angleR);
    float soma = 0.0;
    int numpoints = 0;

    // Rotation matrix
    float zz = cosR , zo = sinR, oz = -sinR, oo = cosR;

    // kernel creation
    for (int x = - dimension_match / 2; x < dimension_match / 2; x++){
        for (int y = - dimension_match / 2; y < dimension_match / 2; y++){
            float u = (x * zz) + (y * oz), su = -(u * u);
            float v = (x * zo) + (y * oo);

            //for the neighbour 
            if (fabs(u) <= (3.0f * sigma) && fabs(v) <= (length / 2.0f)){ 
                kernel.at<float>(x + dimension_match / 2,y + dimension_match / 2) = 
                	- expf(su / sigmasq);
                numpoints += 1;
                soma += kernel.at<float>(x + dimension_match / 2,y 
                	+ dimension_match / 2);
                
            }else{
                kernel.at<float>(x + dimension_match / 2, y + dimension_match / 2) = 0;
            }
        }
    }
    float media = soma / numpoints ;
    
    for (int x = - dimension_match / 2; x < dimension_match / 2; x++){
        for (int y = - dimension_match / 2; y < dimension_match / 2; y++){
            float u1 = (x * zz) + (y * oz);
            float v1 = (x * zo) + (y * oo);
            if (fabs(u1) <= (3.0f * sigma) && fabs(v1) <= (length / 2.0f)){
                kernel.at<float>(x + dimension_match / 2,y + dimension_match / 2) = 
                	roundf(10.0f * (kernel.at<float>(x + dimension_match / 2,y 
                	+ dimension_match / 2) - media));
            }
        }
    }
	return kernel;
}

Mat IMAGE_PROCESSING::correlation_matched(){
    Mat final = Mat::zeros(final_mean.size(), CV_32F);
	// Runs all the image, pixel by pixel, centering the kernel with the pixel,
    // multiplying the repective kernel value for the corresponding image pixel,
    // doing the sum of all the values and putting them into the respective pixel. 
    for (int row = pad; row < height - pad; row++){ 
        for (int col = pad; col < width - pad; col++){           
            float count = 0; 

            for (int i = - dimension_match / 2; i < dimension_match / 2 + 1; i++){
                for (int j = - dimension_match / 2; j < dimension_match / 2 + 1; j++){                    	                
                    count = count + final_mean.at<float>(row + i, col + j) 
                    	* kernel.at<float>(i + dimension_match / 2, 
                    	j + dimension_match / 2);
                }
            final.at<float>(row,col) = count; 
            }
        }
    }
	return final;
}


Mat IMAGE_PROCESSING::maximos_kernel(Mat m1, Mat m2){
	Mat k_max = (Mat::zeros(height, width, CV_32F));
	// Compare the values of the two matrices, pixel by pixel. The bigger value
	// its inserted into the k_max matrix. 
	for (int i = pad; i < height - pad; i++)
		for (int j = pad; j < width - pad; j++){
			int max = maximo(m1.at<float>(i,j), m2.at<float>(i,j));
			k_max.at<float>(i,j) = max;
		}
	return k_max;
}


float IMAGE_PROCESSING::maximo(float a,float b){
	float max;
	if (a > b) {
		max = a;
	} else {
		max = b;
	}
	return max;
}

void IMAGE_PROCESSING::make_threshold() {
	th_image = (Mat::zeros(final_image_matched.size(), CV_32F));
		for (int i = pad; i < height - pad; i++)
        	for (int j = pad; j < width - pad; j++){ 
        		// Sees if the pixel in the image os lower or higher than 
        		// the given threshold.
            	if (final_image_matched.at<float>(i,j) <= threshold){
                	th_image.at<float>(i,j) = 0;
            	} else {
                	th_image.at<float>(i,j) = 255;
            	}
        	}
	}


void IMAGE_PROCESSING::make_mult_mask() {
	mask_fov.convertTo(mask_fov, CV_32F);
    // searches for pixeis with zero value in the FOV mask and 
    //sets the correspondent pixel in the image to zero
	for (int i = pad; i < height - pad; i++)
    	for (int j = pad; j < width - pad; j++){
        	if (mask_fov.at<float>(i,j) == 0) {
            	th_image.at<float>(i,j) = 0;
        	}
    	}
    final_image = th_image;
}
