//
//  PartA.cpp
//  dig
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright © 2018 Naif Alharbi. All rights reserved.
//
#include<iostream>
#include <opencv2/opencv.hpp>
#include<stdint.h>
#include<vector>
#include<algorithm>
using namespace cv;
using namespace std;


Mat encoding(Mat carrier_image, Mat message_input);  // prototype of the encoding function

int main(int argc, char**argv) {
	// if the user didnt input 3 arugments the program will terminate and ask the user to input again

	if (argc != 4) {
	cout << "you need to input for arguments the carrier image(carrier.png) ,message (message.png) and the encoded image new name(encoded.png)" << endl;
		return -1;
	}

	
	
	
	Mat carrier = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	// check the if the name of the carrier image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
	if (carrier.empty()) {
		cout << "Carrier image  " <<argv[1]<<"  not found" << endl;
		return -1;

	}
	cout << "the carrier image  "<< argv[1]<<"uplaoded"<< endl;

	// check the if the name of the message image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
	
	Mat message = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE);

	if (message.empty()) {
		cout <<argv[2] << "not found" << endl;
		return -1;

	}

	cout << argv[2]<<"  uploaded" << endl;

	

	//check the size of the two images , if they dont have the same size the program will be terminated 
	if (carrier.size() != message.size()) {
		cout << "the size is not matched" << endl;
		return -1;

	}

	
	
	Mat encoded = encoding(carrier, message);   // pass the carrier image and the message image to the encoded function parameters
	cout << "The message encoded" << endl;
	
	imwrite(argv[3], encoded);             // save the encoded image
	namedWindow("Encoded image", WINDOW_AUTOSIZE);     //create a window for the encoding image
	namedWindow("Carrier image", WINDOW_AUTOSIZE);     // create a window for the carrier image
	imshow("Carrier image", carrier);                  // display the carrier image
	imshow("Encoded image", encoded);                  // // display the encoded image
	cout << "encoded success and the encoded image saved" << endl;
	waitKey(0);
	destroyWindow("Encoded image");

	return 0;

}



Mat encoding(Mat carrier_image, Mat message_input) {

	threshold(message_input, message_input, 0, 1, THRESH_BINARY);  //apply the threshold in the message image

	message_input = Mat_<uchar>::ones(message_input.size()) - message_input;    


	Mat encoded = carrier_image + message_input;   //the econded message is the result of the carrier+the message
	return encoded;
}