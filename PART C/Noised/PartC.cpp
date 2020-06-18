//
//  PartC.cpp
//  dig
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright © 2018 Naif Alharbi. All rights reserved.
//
//
#include<stdint.h>
#include<opencv2/opencv.hpp>
#include <iostream>
#include<string>
#include<vector>
using namespace std;
using namespace cv;
void guassiannoise(Mat& ,Mat& ,double ,RNG& );
unsigned long hash_djb2(const char *str);
int main(int argc, char*argv[]) {
	// if the user didnt input 2 arugments the program will terminate and ask the user to input again
	if (argc != 3) {
	cout << "you need to input 2 arguments the carrier image ex (carrier.png) and the image with noise name to save it ex (noised.png) " << endl;
		return -1;
	}

	Mat carrier = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	// the if the user has input the correct name of the carrier image or it is exist if not the program will terminated 
	if (carrier.empty()) {
		cout << "Carrier image "<<argv[1] <<" not found" << endl;
		return -1;
	}

	cout << "the carrier image "<<argv[1]<<"  is loaded" << endl;
   

    cout<<"please enter password"<<endl;

    string password;                                           //save the password as string
    getline(cin,password);                                    // the getline will accpet the passowrd from the user to pass it the hash function
    unsigned long seed= hash_djb2(password.c_str());
	Mat noised;                                                   //make a new matrix which will represnt the noised image 
   RNG rng(seed);
  
   guassiannoise(carrier,noised,10,rng);                       // the gaussing function accpet three parametrs the carrier image ,the name of the output
                                                                // image which is noised image , the value of noise or sigma and the rng function
   
	 
 
    imwrite(argv[2],noised);                                 // save the noised image 
	namedWindow("Carrier image", WINDOW_AUTOSIZE);          // create  a window for the carrier image 
	imshow("Carrier image", carrier);                        // display the carrier image
	
	namedWindow("noised image", WINDOW_AUTOSIZE);            // create a window for the noised image
   imshow("noised image",noised);                            // display the noised image
    waitKey(0);                                               
    
    return 0;
    
  
}

//the hash fucntion which use the (djb2) is written from (http://www.cse.yorku.ca/~oz/hash.html)
unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

void guassiannoise(Mat &input_image, Mat &noised_image, double sigma, RNG& rng) {
	
	noised_image = input_image.clone();
	int pixel_value;
	int noise;
	int new_value;
	// the for loop below will access to each pixel in the grayscale image 
	for (int x = 0; x<input_image.rows; x++) {            // access to the image rows
		for (int y = 0; y<input_image.cols; y++) {        // acess to the image columns 
			pixel_value = input_image.at<uchar>(x, y);     // the value of the pixel
			noise = pixel_value + rng.gaussian(sigma);     // apply the noise to the pixel value
			// the if condition below has been made to avoid the overflow of the pixel's value
			if (noise > 255)                              //if the value higher than 255 the new value of the pixel
				new_value = 255;
			          
			else if (noise < 0)                          // if new value less than 0 then new value of te pixel will be 0
				new_value = 0;
			else
				new_value = noise;                       // if the value between 0 and 255 the noise will  be the same
			noised_image.at<uchar>(x, y) = new_value;       // apply the new value of the pixels in the image which called noised image
		}
	}
	
	
}
    
    


