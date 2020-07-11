#ifndef METRICS_H_INCLUDE
#define METRICS_H_INCLUDE

class METRICS {
// In this header it is calculated the metrics associated to the final image, 
// based on a comparison with the veins mask.
// Thus, the true positives and negatives and false positives and 
// negatives are calculated in order to obtain metrics like accuracy
// sensitive and specificity.
// Input arguments: Image to be avaluated, the image padding, the image directory,
// the final directory and the number of the trial.
// Output arguments: true positives, true negatives, false positives, false negatives
// accuracy, sensibility and specificity.
public:
	METRICS(cv::Mat image, cv::Mat mask_V, cv::Mat mask_FOV, int padding, std::string dir, 
		std::string count){
		image_original = image;
		pad = padding;
		out_dir = dir;
		number = count;
		mask_veins = mask_V;
		mask_fov = mask_FOV;
		lines = (size_t)image.size().height;
		colunes = (size_t)image.size().width;
		metrics();
	}

	// Print the true positives and negatives
	// and the false positives and negatives
	void print_metrics(void) const ;

	// Calculates and prints accuracy, sensitive and specificity
	void print_performance(void);


	// Create a .txt file with the metrics
	void print_txt_metrics(void);

private:
	std::string number;  // Number of the trial
	std::string out_dir; // Final directory
	std::string id_im;  // Image directory
	float TP;  // True positives
	float TN;  // True negatives
	float FP;  // False positives
	float FN;  // False negatives
	float accuracy;  
	float precision;
	float sensitive;
	float specificity;
	int pad;  // Applied padding
	int lines;  // Lines of the image
	int colunes;  //  Colunes of the image 
	cv::Mat image_original;
	cv::Mat mask_veins; 
	cv::Mat mask_fov;

	// Calculates the true positives and negatives
	// and the false positives and negatives
	void metrics(void);
};

	
#endif