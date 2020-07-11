#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>  
#include <sstream>
#include "metrics.h"


using namespace cv;
using namespace std;


void METRICS::metrics(void) {
	TP = 0;
	TN = 0;
	FP = 0;
	FN = 0;
	mask_veins.convertTo(mask_veins, CV_32F);
	mask_fov.convertTo(mask_fov, CV_32F);

	// If the image pixel is 255  and the same pixel in the mask
	// is 255 it's a TP, otherwise it's FP.  If the image pixel is
	// 0 and the same mask pixel is also 0 it's considered a TN.  
	// however if the pixel in the mask is 255, then it's a FN.
	for (int i = pad; i < lines - pad; i++)
		for (int j = pad; j < colunes - pad; j++){
			if (mask_fov.at<float>(i,j) == 255){
				if (image_original.at<float>(i,j) == 255){
					if (mask_veins.at<float>(i,j) == 255) {
						TP = TP + 1;
					} else if (mask_veins.at<float>(i,j) == 0){
						FP = FP + 1;
					}
				} else if (image_original.at<float>(i,j) == 0){
					if (mask_veins.at<float>(i,j) == 0){
						TN = TN + 1;
					} else if (mask_veins.at<float>(i,j) == 255){
						FN = FN + 1;
					}
				}
			}	
		}
}


void METRICS::print_txt_metrics(void) {
	string name = out_dir + string("metrics ") + number + string(".txt");
	ofstream outfile (name);
	outfile << "--------------------------------" << endl;
	outfile << "True positives:  " << TP << endl;
	outfile << "True negatives:  " << TN << endl;
	outfile << "False positives: " << FP << endl;
	outfile << "False negatives: " << FN << endl;
	outfile << "--------------------------------" << endl;
	outfile << "Accuracy:    " << accuracy << endl;
	outfile << "Sensitive:   " << sensitive << endl;
	outfile << "Specificity: " << specificity << endl;
	outfile << "--------------------------------" << endl;
	outfile << "Program made by:                " << endl;
	outfile << "Sara Cerqueira and Simão Santos" << endl;

	outfile.close();
}

// The several metrics, all calculated as its shown above.
void METRICS::print_performance(void) {
	accuracy = ((TP + TN) / (TP + TN + FP + FN)) * 100;
	sensitive = (TP / (TP + FN)) * 100;
	precision = (TP / (TN + FP)) * 100;
	specificity = (TN / (TN + FP)) * 100;

	cout << "Accuracy:    " << accuracy << endl;
	cout << "Sensitive:   " << sensitive << endl;
	cout << "Specificity: " << specificity << endl << endl;
}

void METRICS::print_metrics(void) const {
	cout << "True positives:  " << TP << endl;
	cout << "True negatives:  " << TN << endl;
	cout << "False positives: " << FP << endl;
	cout << "False negatives: " << FN << endl;
}

