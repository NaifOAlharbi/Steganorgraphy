
//
//  Created by Naif Alharbi on 24/11/2018.
//  Copyright � 2018 Naif Alharbi. All rights reserved.
#include<opencv2/opencv.hpp>
#include <iostream>
#include<string>
#include<algorithm>

using namespace std;
using namespace cv;

unsigned long hash_djb2(const char*str); // protoype of the hash _djb2 function
Mat decoded(Mat carrier_image, Mat encoded_image);      // prototype of the decoding function

int main(int argc, char*argv[]) {
	//if the user didnt input 3 parameters the program will terminated and will ask the user to input again
	if (argc != 4) {
		cout <<"you need to input for arguments the carrier image(carrier.png) ,encoded (encoded.png) and the decoded image new name(decoded.png)" << endl;
		return -1;
	}


	
	Mat carrier = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);                   // read the carrier image in grayscale
	//cout << name1 << endl;
	// check the if the name of the carrier image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated
	if (carrier.empty()) {
		cout << "Carrier image "  << argv[1] <<" not found" <<endl;
		return -1;

	}
	cout << argv[1] << " is loaded  " << endl;


	
	Mat encoded = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);                      //read the encoded image in grayscale
	if (encoded.empty()) {
		cout << " the encoded image " <<argv[2]<<" not found" << endl;
		return -1;

	}

	cout <<argv[2]<<" is loaded" << endl;



	// check the if the name of the encoded image is right and the file is exist or if the user didn't type anything it means empty and the program 
	// will be terminated

	//check the size of the two images , if they dont have the same size the program will be terminated 
	if (carrier.size() != encoded.size()) {
		cout << "the size is not matched" << endl;
		return -1;

	}


	//save the entered password as string
	string password;
	cout << "enter the password" << endl;

	getline(cin, password);                   // getline will take the password from the user and save it to pass to the hash function
	cout << "your password is " << password << endl;
	
	
	//save the entered password as string
	auto seed = hash_djb2(password.c_str());             
	RNG rng(seed);                                      //Generate random numbers for seed function
	Mat decoding = decoded(carrier, encoded);         // apply the decdoing function which will take two parametrs the carrier image and the encoded image
	
					 //flatten and shuffle order
	vector<Point2i> indexs(decoding.cols*decoding.rows);
	vector<int> random_idx(decoding.cols*decoding.rows);
	int index = 0;
	for (int x = 0; x < decoding.rows; x++) {           //acces to the encoded image rows
		for (int y = 0; y < decoding.cols; y++) {       // acess to the encoded image columns
			indexs[index] = Point2i(x, y);
			int num = rng.uniform(0, decoding.rows*decoding.cols);           
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
	// shuffle between image elements in reverse to decode the image
	int i = indexs.size()-1;
	for (int x = decoding.rows-1; x >=0; x--) {
		for (int y = decoding.cols-1; y >=0; y--) {
			int V = decoding.at<uchar>(x, y);
			decoding.at<uchar>(x, y) = decoding.at<uchar>(indexs[i].x, indexs[i].y);
			decoding.at<uchar>(indexs[i].x, indexs[i].y) = V;
			i--;
		}
	}
	cout << "the image is decoded and the message is shown in the window" << endl;
	
	
	imwrite(argv[3], decoding);                               // save the decoded message according to the user input
	namedWindow("decoded", WINDOW_AUTOSIZE);                  // create a window for the decoded message
	imshow("decoded", decoding);                              // display the decoded message
	waitKey(0);

	return 0;
}


////the hash fucntion which use the (djb2) is written from   (http://www.cse.yorku.ca/~oz/hash.html)
unsigned long hash_djb2(const char* str) {

	unsigned long hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c;
	return hash;
}






Mat decoded(Mat carrier_image, Mat encoded_image) {
	Mat decoding = Mat_<uchar>::ones(carrier_image.size());   // set the decoded message size to 1 according to the carrier image size
	decoding -= encoded_image - carrier_image;  // subtract the encoded image from the carrier image to obatin the message
	decoding *= 255;                            // adjust the dark pixles to white
	return decoding;
}

