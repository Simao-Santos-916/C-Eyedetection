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
#include "image.cpp"
#include "image_processing.h"
#include "image_processing.cpp"
#include "metrics.h"
#include "metrics.cpp"


using namespace cv;
using namespace std;

int main(){
	int work = 0;
	string image_id;
	string mask_FOV_id;
	string mask_V_id;
	string drive;
	string type_im;
	string final_directory;
	int pad;
	int dimensions_med;
	int dimensions_matched;
	int length = 9;
	int sigma = 2;
	string next;
	int th;
	string option;
	int count = 0;


	cout << "Directory of the DRIVE" << endl;
	cin >> drive;
	cout << "Do you want test or trainning images" << endl;
	cout << "For test - 1, for trainning - 2" << endl;
	int tt;
	cin >> tt;
	if (tt == 1) {
		type_im = "test";
	} else {
		type_im = "training";
	}

	cout << "Do you wanna save your image else were?  yes (y), no (n)" << endl;
	string opt3;
	cin >> opt3;
	if (opt3 == "y") { 
		cout << "Insert directory below" << endl;
		cin >> final_directory;
	} else if (opt3 == "n") {
		final_directory = drive;
	}


	while (work == 0){
		if (count >= 1){
			cout << "Do you want to change the test and trainning" << endl;
			string opt4;
			cin >> opt4;
			if (opt4 == "y"){
				cout << "For test - 1, for trainning - 2" << endl;
				cin >> tt;
				if (tt == 1){
					type_im = "test";
				} else {
					type_im = "training";
				}
			}
		}

		cout << "Put the id image" << endl;
		cout << "Please, always insert two digits" << endl;
		cout << "E.g. 01 or 21" << endl;
		cin >> image_id;



		count = count + 1;
		string count_id = to_string(count);
		cout << "what's the size of the mean filter?" << endl;
		cin >> dimensions_med;
		cout << "what's the size of the matched filter kernels?" << endl;
		cin >> dimensions_matched;
		cout << "What's the length you wanna use?" << endl;
		cin >> length;
		cout << "What's the sigma value?" << endl;
		cin >> sigma;
 	
		string image_name = drive + string(type_im) + string("/images/") 
			+ image_id + string("_") + string(type_im) + string(".tif"); 
		string mask_FOV_name = drive + string(type_im) + string("/mask/")
			+ image_id + string("_") + string(type_im) + string("_mask.png");
		string mask_V_name = drive + string(type_im) + string("/1st_manual/")
			+ image_id + string("_manual1.png");

		if (dimensions_med > dimensions_matched) {
			pad = dimensions_med;
		}  else {
			pad = dimensions_matched;
		}

		IMAGE *I1 = new IMAGE(image_name, mask_FOV_name, mask_V_name, pad);

		Mat image_original = I1->get_im();
		Mat mask_FOV = I1->get_mask_fov();

		cout << "Which threshold do you wanna use?" << endl;
		cin >> th;

		cout << "Please wait" << endl << endl;

		
		clock_t t;

		IMAGE_PROCESSING *IP = new IMAGE_PROCESSING(image_original, mask_FOV, 
			dimensions_med, dimensions_matched, image_id, final_directory, 
			pad, length, sigma, th);

		cout << "Saved mean filtered image" << endl;
		cout << "Saved convolved image" << endl;
		cout << "Saved final image" << endl; 

		Mat final_image = IP->get_image_final();
		Mat mask_veins = I1->get_mask_v();
		cout << "------------------------------------------" << endl << endl;
		METRICS *ME = new METRICS(final_image, mask_veins, mask_FOV, pad, 
			final_directory, image_id);
		ME->print_metrics();
		ME->print_performance();
		ME->print_txt_metrics();
		cout << "-------------------------------------------" << endl;
		cout << "It was also printed a .txt file for you to see the metrics" << endl << endl;
		t = clock();
	    cout << "It took " << t/CLOCKS_PER_SEC << " seconds to do all of this." << endl << endl; 
	    cout << "Do you want to see the kernels? yes (y)  no (n)" << endl;
    	string opt;
    	cin >> opt;
    	if (opt == "y") {
    		cout << "Print all, or just one? All - a, one - o" << endl;
    		string opt1;
    		cin >> opt1;
    		if (opt1 == "a"){
    			IP->print_kernels();
    			cout << "Your kernels were saved in a .txt file" << endl;
    		} else if (opt1 == "o") {
    			cout << "Which angle in degrees?" << endl;
    			int angle;
    			cin >> angle;
    			IP->print_kernel(angle); 
    		} else {
    			cout << "Fail, insert a valid option next time" << endl;
    		}
    	}
	    cout << "Do it again? yes (y) no (n)" << endl << endl;
	    cin >> next;
	    if (next == "n") { 
	    	cout << "Closed program" << endl;
	    	work = 1; 
	    } else if (next == "y") {
			cout << "//----//----//----//----//----//----//----//" << endl;
	    	work = 0; 
		} else {
			cout << "Insert a valid option!" << endl;
		}
	}	
	return 0;
}