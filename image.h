#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

class IMAGE {
// This header open the images required, and the masks. 
// Input: image, FOV mask and veins mask directory, and the padding.
// Output: Image, FOV mask and mask veins.
public:
	IMAGE(std::string dir_im_u, std::string dir_fov_mask, std::string dir_mak_V, 
		int padding) {
		dir_im = dir_im_u;
		dir_mask_FOV = dir_fov_mask;
		dir_mask_v = dir_mak_V;
		pad = padding;
	}


	// Get image
	cv::Mat get_im(void);


	// Get FOV mask
	cv::Mat get_mask_fov(void);


	// Get veins mask
	cv::Mat get_mask_v(void);


	// Call to now which size of padding your using
	int get_pad(void) const;

private:
	cv::Mat imW;  // Original image
	cv::Mat mask_V;  // Veins mask
	cv::Mat mask_FOV;  // FOV mask
	int pad;  // Padding value
	int lines;  // Image lines
	int colunes;  // Image colunes
	std::string dir_im;  // Image directory
	std::string dir_mask_FOV;  // FOV mask directory
	std::string dir_mask_v;  // Veins mask directory
};

#endif