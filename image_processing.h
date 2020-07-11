#ifndef MEAN_FILTER_H_INCLUDE
#define MEAN_FILTER_H_INCLUDE

class IMAGE_PROCESSING {

public: 
	IMAGE_PROCESSING(cv::Mat image, cv::Mat mask, int n1, int n2, std::string count, 
		std::string out, int padding, int L, int sigma_, int th) {
	pad = padding;
	image_o = image;
	dimension_mean = n1;
	length = L;
	sigma = sigma_;
	dimension_match = n2;
	threshold = th;
	mask_fov = mask;
	dir_out = out;
	number = count;
	height = (size_t)image.size().height;
	width = (size_t)image.size().width;  
	print_mean();
	print_image_matched();
	print_image_final();
	}


	// Get the used kernel
	Mat get_kernel_mean(void);


	// Print the mean filter image
	void print_mean(void);


	// Print all kernels of the matched filter to file
	void print_kernels(void);


	// Print one especific kernel of the matched filter
	void print_kernel(int theta);


	// Print the convolved image
	void print_image_matched(void);


	// Print final image
	void print_image_final(void);

	// Get the final image
	Mat get_image_final(void) const;


private:
	int pad;  // Used padding
	cv::Mat image_o;  // Original image
	int dimension_mean;  // Mean kernel dimension
	cv::Mat final_mean;  // Mean filter result
	cv::Mat kernel;  // Kernels used in Matched filter process
	int angle;  // Angle to create kernels
	cv::Mat final_image_matched;  // Matched filter image result
	int length;  // Length to the kernels creation
	int sigma;  // Sigma to the kernels creation
	int dimension_match;   // Matched filter kernels size
	int threshold;  // Used threshold
	cv::Mat final_image;  // Result image
	cv::Mat mask_fov;  // Used FOV mask
	cv::Mat th_image;  // Result image from the threshold
	std::string dir_out;  // Save images directory
	std::string number;  // Number of trials
	int height;  // Lines on the image
	int width;   // Colunes on the image

	void correlation_mean();


	// Call to do all the Matched Filter process.
	// Fuction called to make all the process. For 1 to 12, creats the kernel, apply
	// the convolution and makes the non maximus supression.
	// Output: convolved image.
	void correlation_cycle(void);


	// Creates the kernels, can choose de length and the dimension.
	// Based in the work of S. Chaudhuri.
	// Output: kernels.
	Mat kernel_fabrication(void);


	// Applied correlation for the matched filter application. 
	Mat correlation_matched(void);

	// Maximos get fuction
	Mat maximos_kernel(Mat m1, Mat m2);


	// Choose which it's bigger
	float maximo(float a,float b);

	// Makes binarization.
	// Sets the image pixels to zero if the original value 
	// is lower than the choosen threshold. 
	// Sets the other pixels to 255.
	void make_threshold(void);

	// Eliminates the border of the image.
	// When the FOV mask is 0, sets the image to 0
	void make_mult_mask(void);

};
#endif