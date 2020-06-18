
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright � 2018 Naif Alharbi. All rights reserved.
#include<opencv2/opencv.hpp>
#include <iostream>
#include<string>
#include<vector>

using namespace std;
using namespace cv;

unsigned long hash_djb2(const char*str); // protoype of the hash _djb2 function



Mat encoding(Mat carrier_image, Mat message_input); // prototype of the encoding function
int main(int argc, char*argv[]) {
	// if the user didnt input 3 arugments the program will terminate and ask the user to input again
	if (argc != 4) {
		cout << "you need to input for arguments the carrier image(carrier.png) ,message (message.png) , the encoded image new name(encoded.png)" << endl;
		return -1;
	}


	
	
	Mat carrier = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // the carrier image in grayscale
	
	// check the if the name of the carrier image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
	if (carrier.empty()) {
		cout << "Carrier image not found"<<"  " <<argv[1]<< endl;
		return -1;

	}
	cout << argv[1] << " uploaded  " <<endl;


	
	Mat message = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);    // read the message in grayscale

	if (message.empty()) {
		cout << "Message image not found"<<" "<<argv[2] << endl;
		return -1;

	}

	cout << argv[2] <<"    uploaded  "<< endl;

	// check the if the name of the message image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated

	//check the size of the two images , if they dont have the same size the program will be terminated 
	if (carrier.size() != message.size()) {
		cout << "the size is not matched" << endl;
		return -1;

	}


	//save the entered password as string
	string password;
	cout << "enter the password" << endl;

	getline(cin, password);                           //get line will promot the user to input the password and save this password
	cout << "your password is   " << password  << "  please remember it to use it to decode the message" << endl;
	

	auto seed = hash_djb2(password.c_str());            // pass the password in the uash function
	RNG rng(seed);                                        //Generate random numbers for seed function
	
	//flatten and shuffle order
	vector<Point2i> indexs(message.cols*message.rows);        // adjust a vector has the same size as the message
	vector<int> random_idx(message.cols*message.rows);         
	int index = 0;
	for (int x = 0; x < message.rows; x++) {                   // acess to the message rows
		for (int y = 0; y < message.cols; y++) {               // access the message columns 
			indexs[index] = Point2i(x, y);                     
			int num = rng.uniform(0, message.rows*message.cols);   // generats  random numbers accrording to the message size
			random_idx[index] = num;
			index++;
		}
	}
	// shuffle bteween the vector indexs and the random_idx
	for (int i = 0; i < indexs.size(); i++) {
		Point2i temp = indexs[i];
		indexs[i] = indexs[random_idx[i]];
		indexs[random_idx[i]] = temp;
	}
	// shuffle between image elements
	int i = 0;
	for (int x = 0; x < message.rows; x++) {
		for (int y = 0; y < message.cols; y++) {
			int V = message.at<uchar>(x, y);
			message.at<uchar>(x, y) = message.at<uchar>(indexs[i].x, indexs[i].y);
			message.at<uchar>(indexs[i].x, indexs[i].y) = V;
			i++;
		}
	}
	Mat encoded = encoding(carrier, message);            // apply the encoding function and pass two parameters the carrier image and the message
	cout << "The Message encoded" << endl;

	


	
	imwrite(argv[3], encoded);                                         // save the encoded image according to the user input  
	namedWindow("Carrier image", WINDOW_AUTOSIZE);                     // create a window for the carrier image
	imshow("Carrier image", carrier);                                  // display the carrier image
	namedWindow("Encoded image", WINDOW_AUTOSIZE);                      // create  a window for the encoded image
	imshow("Encoded image", encoded);                                   // display the encoded image
	waitKey(0);

	return 0;



}
	

//the hash fucntion which use the (djb2) is written from (http://www.cse.yorku.ca/~oz/hash.html)
unsigned long hash_djb2(const char*str) {

	unsigned long hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c;  //hash*33+c 
	return hash;

}
Mat encoding(Mat carrier_image, Mat message_input) {

	threshold(message_input, message_input, 0, 1, THRESH_BINARY);  // change the all zeros to ones and vice versa

	message_input = Mat_<uchar>::ones(message_input.size()) - message_input;


	Mat encoded = carrier_image + message_input;   //the econded message is the result of the carrier+the message
	return encoded;
}
