//
//  PartA.cpp
//  dig
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright � 2018 Naif Alharbi. All rights reserved.


#include<iostream>
#include <opencv2/opencv.hpp>
#include<stdint.h>
#include<vector>
#include<algorithm>
using namespace cv;
using namespace std;


Mat decoded(Mat carrier_image, Mat encoded_image);
int main(int argc,char **argv) {
	// if the user didnt input 3 arugments the program will terminate and ask the user to input again
	if (argc != 4) {
		cout << "you need to input for arguments the carrier image(carrier.png) ,encoded (encoded.png) and the decoded image new name(decoded.png)" << endl;
		return -1;
	}

	
	Mat carrier = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); // get the carrier image and convert it to grayscale
// check the if the name of the carrier image is right and the file is exist or if the user didn't type anything it means empty and the program 
// will be terminated
	if (carrier.empty() ) {
		cout << "Carrier image   " <<argv[1]<<"   updloaded"<< endl;
		return -1;
		
	}
	cout << "the carrier image  "<<argv[1]<<"  updloaded" << endl;

	
	Mat encoded = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);   // get the encoded image and convert it to grayscale image
	// check the if the name of the encoded image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
		if (encoded.empty()) {
			cout << "Encoded image  "<<argv[2]<<"  not found" << endl;
			return -1;
		}

		cout << "the encoded image  " << argv[2]<< "  loaded" << endl;
		//check the size of the two images , if they dont have the same size the program will be terminated 
	if (carrier.size() != encoded.size()) {
		cout << "the size is not matched" << endl;
		return -1;

	}


	Mat decoded_image = decoded(carrier, encoded);        // apply the carrier images and the encoded image the decoded_ image function 
	cout << "The image decoded and the message shown in the window" << endl;
	

	imwrite(argv[3], decoded_image);  // save the decoded_image which is the message 
	namedWindow("message", WINDOW_AUTOSIZE);
	imshow("message", decoded_image);                             // show the message
	waitKey(0);                                                   // wait the user to press any input 
	destroyWindow("decoded image ");                               // close the window when the user press any key
	return 0;
}
	
	Mat decoded(Mat carrier_image, Mat encoded_image) {
		Mat decoding = Mat_<uchar>::ones(carrier_image.size());   // set the decoded message size to 1 according to the carrier image size
		decoding -= encoded_image - carrier_image;  // subtract the encoded image from the carrier image to obatin the message
		decoding *= 255;                            // adjust the dark pixles to white
		return decoding;
	}
	
