
//PartD.cpp
//  dig
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright © 2018 Naif Alharbi. All rights reserved.
#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

unsigned long hash_djb2(const char *str);
void guassiannoise(Mat_<Vec3b> &, Mat_<Vec3b> &, double, RNG&);

int main(int argc, char**argv) {
	// if the user didnt input 3 arugments the program will terminate and ask the user to input again
	if (argc != 4) {
		cout << "you need to input for arguments the carrier image(carrier.png) ,message (message.png) , the encoded image new name(encoded.png)" << endl;
		return -1;
	}



	Mat_<Vec3b> carrier = imread(argv[1],CV_LOAD_IMAGE_COLOR);

	// check the if the name of the carrier image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
	if (carrier.empty()) {
		cout << "Carrier image not found" << "  " << argv[1] << endl;
		return -1;

	}
	cout << argv[1] << " uploaded  " << endl;



	auto message = Mat_<uchar>(imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE));

	if (message.empty()) {
		cout << "Message image not found" << " " << argv[2] << endl;
		return -1;

	}

	cout << argv[2] << " uploaded  " << endl;

	// check the if the name of the message image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated

	//check the size of the two images , if they dont have the same size the program will be terminated 
	if (carrier.size() != message.size()) {
		cout << "the size is not matched" << endl;
		return -1;

	}

	
	
	string password;                                         //save the entered password as string
	cout << "Enter the password" << endl;

	getline(cin, password);                     //get line will promot the user to input the password and save this password                           
	cout << "Your password is " << password << " please remember it to decode the image " << endl;
	unsigned long seed = hash_djb2(password.c_str());
	double sigma = 5;                                   // the value of suitable noise
	RNG rng(seed);
	
	Mat_<Vec3b> noised;                                           //make a new matrix which will represnt the noised image
	guassiannoise(carrier, noised, sigma, rng);                 // the gaussing function accpet three parametrs the carrier image ,the name of the output
                                                                // image which is noised image , the value of noise or sigma and the rng function
	enum condition{unused,used};                                 // create a mtrix which shows the sate of each pixel in carrier image 
	Mat_<Vec3b> state(carrier.size(), Vec3b::all(unused));       // if it was used or not
	auto encoded = carrier.clone();                  
	for (auto& pixel : message) {                              //  random values of rows, columns and channels will be generated till the
		                                                       // pixel is not used
		uchar* location;
		int col, row, channel;
		do {
			row = rng(carrier.rows);                 
			col = rng(carrier.cols);
			channel = rng(3);                       //number of channels
			location = &carrier.at<Vec3b>(row, col)[channel];   // the location variable will store the value of each channel
		} while (state.at<Vec3b>(row, col)[channel] == used || *location == 255); // the loop will keep running if the pixel is used
			                                                                      // or =255
		encoded.at<Vec3b>(row, col)[channel] += pixel ? 0 : 1;
		state.at<Vec3b>(row, col)[channel] = used;
	}
	cout << "The Message encoded" << endl;
	
				
		


	

	imwrite(argv[3], encoded);                           // save the encoded image name as the user's input
	namedWindow("Carrier image", WINDOW_AUTOSIZE);        //  create a window for the carrier image
	imshow("Carrier image", carrier);                     // display the carrier image
	namedWindow("Encoded image", WINDOW_AUTOSIZE);              //  create a window for the encoded image
	imshow("Encoded image", encoded);                            // display the Encoded image
	waitKey(0);

	return 0;
}


unsigned long hash_djb2(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

void guassiannoise(Mat_<Vec3b> &input_image, Mat_<Vec3b> &noised_image, double sigma, RNG& rng) {

	noised_image = input_image.clone();
	int noise;
	int pixel;
	////access to the image elements for the colour image
	for (int x = 0; x < input_image.rows; x++) {
		for (int y = 0; y < input_image.cols; y++) {
			for (int i = 0; i < 3; i++) {                            //access to each channel in the pixel the index from 1 to 3 becuase of 
				                                                     // each pixel has three channels BGR B=0 G=1 R=2

				pixel = input_image.at<Vec3b>(x, y)[i];  //the value of each channel

				noise = pixel + rng.gaussian(sigma);     // apply the noised in the pixel elements

				if (noise > 255)                          //avoding the overflow of the pixels elements which will ouccred because of
					pixel = 255;                          // the noise so, if the value higher than 255 it will adjusted at 255
				else if (noise < 0)                       // if less than zero it will be adjusted at zero
					pixel = 0;                           
				else
					pixel = noise;                        // the vlaue will be as the noise if the value between 0 and 255

				noised_image.at<Vec3b>(x, y)[i] = pixel;      // apply the noise to the image
			}
		}
	}
}

