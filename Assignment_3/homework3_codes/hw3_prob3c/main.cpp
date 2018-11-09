//Link for code----https://docs.opencv.org/3.3.1/d5/d6f/tutorial_feature_flann_matcher.html

#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <math.h>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();
int* calculate_l2(Mat descriptor, Mat vocabulary) {
    double *euclideanDist = new double[vocabulary.rows];
    double currentDist = 0.0;
    double minDist = 0;
    int index = 0;
    int vocabRows = vocabulary.rows;

    int *frequency = new int[vocabRows];
    for (int i = 0; i < vocabRows;i++) {
        frequency[i] = 0;
    }

    for (int i = 0; i < descriptor.rows; i++) {
        for (int k = 0; k < vocabulary.rows;k++) {
            for (int j = 0; j < descriptor.cols; j++) {
                double intDiff = (descriptor.at<float>(i,j)-vocabulary.at<float>(k,j));
                currentDist += pow(intDiff , 2);
            }
            euclideanDist[k] = (sqrt(currentDist));
            currentDist = 0.0;
        }
        minDist = euclideanDist[0];
        index = 0;

        for (int k = 1; k<vocabulary.rows; k++) {
            if (minDist>euclideanDist[k]) {
                minDist = euclideanDist[k];
                index = k;
                //	cout<<index<<endl;
            }
        }	//cout<<"temp:"<<temp<<endl;
        frequency[index] += 1;
        //	cout << "The Texture" << i << " :is mapped to following index:" << mappedIndex[i] << endl;

    }
    return frequency;
}

void writetoFile(string path,int *vocab_img,int numRows) {
    ofstream arrayData;
    arrayData.open(path);
    if (arrayData.is_open()) {
        for (int i = 0; i<numRows; i++) {
            arrayData << vocab_img[i];
            arrayData << "\n";
        }
        arrayData.close();
    }
    else
    {
        cout << "unable to open file" << endl;
    }//original histogram to text file
}

double compare_codewords(int *vocab_img,int *ferarri2) {
    double error = 0;
    for (int j = 0; j < 8;j++) {
        error += abs(vocab_img[j]-ferarri2[j]);
    }
    error =error/8.0;
    return error;
}

int main( int argc, char** argv )
{

    if( argc != 5 )
    { readme();
        return -1; }


    //!!!!!Change reading of images according to the question

    //read my images

    Mat img_1 = imread( argv[3], CV_LOAD_IMAGE_COLOR );//Ferrari_1
    Mat img_2 = imread( argv[4], CV_LOAD_IMAGE_COLOR );//Ferrari_2
    Mat img_3 = imread( argv[1], CV_LOAD_IMAGE_COLOR );//Optimus prime
    Mat img_4 = imread( argv[2], CV_LOAD_IMAGE_COLOR );//Bumblebee

    if( !img_1.data || !img_2.data || !img_3.data || !img_4.data)
    {
        std::cout<< " Images not found. Check the path for the images " << std::endl;
        return -1;
    }

    //WHat I understand is - find SIFT features  - codewords of all the 4 images and match the codeword of ferrari2 with rest of them
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    //Now keypoint descriptor is created. A 16x16 neighbourhood around the keypoint is taken. It is devided into 16 sub-blocks of 4x4 size. For each sub-block, 8 bin orientation histogram is created. So a total of 128 bin values are available. It is represented as a vector to form keypoint descriptor. In addition to this, several measures are taken to achieve robustness against illumination changes, rotation etc.
    //Keypoints between two images are matched by identifying their nearest neighbours. But in some cases, the second closest-match may be very near to the first. It may happen due to noise or some other reasons. In that case, ratio of closest-distance to second-closest distance is taken. If it is greater than 0.8, they are rejected. It eliminaters around 90% of false matches while discards only 5% correct matches, as per the paper.
    //sift.detect() function finds the keypoint in the images.
    //cv2.drawKeyPoints() function which draws the small circles on the locations of keypoints.

    //sift.compute() which computes the descriptors from the keypoints

    int minHessian = 400;

    //Ptr<SURF> detector = SURF::create(minHessian);
    Ptr<SIFT> Siftdetector = SIFT::create( minHessian );

    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    Mat image_keypoints_1, image_keypoints_2;

    Siftdetector->detect(img_1, keypoints_1);
    Siftdetector->detect(img_2, keypoints_2);

    //Now draw keypoints
    drawKeypoints(img_1, keypoints_1, image_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(img_2, keypoints_2, image_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    //-- Show detected (drawn) keypoints
   //imshow("Sift", image_keypoints_1);
   // waitKey();
   // imshow("Sift", image_keypoints_2);
   // waitKey();


   //Now find the descriptors
    Siftdetector->compute(img_1, keypoints_1, descriptors_1);
    Siftdetector->compute(img_2, keypoints_2, descriptors_2);

    //now we use BFMatcher which is the brute force matcher - so it rigorously compares the matching points
    //it performs distanace calculation and by default it is L2 norm
    //so now we'll declare an object of it

    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1,descriptors_2,matches);

    //Now draw the matching points
    Mat matchDespcriptor;
    drawMatches(img_1,keypoints_1,img_2,keypoints_2,matches,matchDespcriptor);
    imshow("SIFT Matching Keypoints",matchDespcriptor);
    waitKey();

    ////Now for BAg of words you need all the sift features of all the images

   //////pre - BAG OF WORDS//////////////////////////////////////////////////////
    vector<KeyPoint> keypoints_3, keypoints_4;
    Mat img_keypoints_3; Mat img_keypoints_4;
    Mat descriptors_3, descriptors_4;

    Siftdetector->detect(img_3, keypoints_3);
    Siftdetector->detect(img_4, keypoints_4);

    drawKeypoints(img_3, keypoints_3, img_keypoints_3, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(img_4, keypoints_4, img_keypoints_4, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

   // imshow("SIft 3", img_keypoints_3);
   // imshow("Sift 4", img_keypoints_4);


    Siftdetector->compute(img_3, keypoints_3, descriptors_3);
    Siftdetector->compute(img_4, keypoints_4, descriptors_4);
    matcher.match(descriptors_1, descriptors_3, matches);
    drawMatches(img_1, keypoints_1, img_3, keypoints_3, matches, matchDespcriptor);
	//imshow("Image 1 and 3", matchDespcriptor);
    //waitKey();
    matcher.match(descriptors_2, descriptors_4, matches);
    drawMatches(img_2, keypoints_2, img_4, keypoints_4, matches, matchDespcriptor);
   // imshow("Image 2 and 4", matchDespcriptor);

    ////// BAG OF WORDS//////////////////////////////////////////////////////
    BOWKMeansTrainer BGOWORDS(8);//you need 8 clusters
    BGOWORDS.add(descriptors_1); //adds descriptors to a training set
    BGOWORDS.add(descriptors_3);
    BGOWORDS.add(descriptors_4);

    Mat vocabulary = BGOWORDS.cluster(); //k means clustering UNSURE if it is default CHECK!

    //in vocabulary each row will be a descriptor belonging to that image number

    // declare variables
    int *vocab_img1 = new int[vocabulary.rows];
    int *vocab_img2 = new int[vocabulary.rows];
    int *vocab_img3 = new int[vocabulary.rows];
    int *vocab_img4 = new int[vocabulary.rows];

    vocab_img1 = calculate_l2(descriptors_1,vocabulary);
    vocab_img2 = calculate_l2(descriptors_2, vocabulary);
    vocab_img3 = calculate_l2(descriptors_3, vocabulary);
    vocab_img4 = calculate_l2(descriptors_4, vocabulary);

    writetoFile("/Users/mansi/Documents/imageProcessing/images/EE569_hw3_images/histCount1.txt", vocab_img1, 8);
    writetoFile("/Users/mansi/Documents/imageProcessing/images/EE569_hw3_images/histCount2.txt", vocab_img2, 8);
    writetoFile("/Users/mansi/Documents/imageProcessing/images/EE569_hw3_images/histCount3.txt", vocab_img3, 8);
    writetoFile("/Users/mansi/Documents/imageProcessing/images/EE569_hw3_images/histCount4.txt", vocab_img4, 8);


    //Now compare ferrari 2 codewords with others

    double error1 = compare_codewords(vocab_img1,vocab_img2);
    double error2 = compare_codewords(vocab_img3, vocab_img2);
    double error3 = compare_codewords(vocab_img4, vocab_img2);
    cout << "Comparing Ferrari2 with Ferrari1: The error is: "<< error1 << endl;
    cout << "Comparing Ferrari2 with OptimusPrime: The error is: "<<error2 << endl;
    cout << "Comparing Ferrari2 with Bumblebee: The error is: "<< error3 << endl;

    if(error1 < error2 && error1 < error3){
        cout<<" Ferrari 2 is closest to Ferrari 1"<<endl;
    }

    if(error2 < error3 && error2 < error1){
        cout<<" Ferrari 2 is closest to  Optimus prime"<<endl;
    }

    if(error3 < error2 && error3 < error1){
        cout<<" Ferrari 2 is closest to Bumblebee"<<endl;
    }



    return 0 ;

}


void readme()
{ std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }