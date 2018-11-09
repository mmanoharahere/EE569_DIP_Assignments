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

int main( int argc, char** argv )
{

    if( argc != 3 )
    { readme();
        return -1; }



    //read my images

    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );//Optimus prime
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );//Bumblebee


    if( !img_1.data || !img_2.data)
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

    int minHessian = 100;

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
    imshow("Sift", image_keypoints_1);
     waitKey();
     imshow("Sift", image_keypoints_2);
     waitKey();







/////////////SURF//////////////////////////////////////////////////////////

    //Repeat the same for SURF
    Ptr<SURF> surfDetector = SURF::create(minHessian);
    vector<KeyPoint> surfKeypoints_1, surfKeypoints_2;
    Mat surfImg_keypoints_1; Mat surfImg_keypoints_2;
    Mat surfMatchDespcriptor; vector<DMatch> surfMatches;

    surfDetector->detect(img_1, surfKeypoints_1);
    surfDetector->detect(img_2, surfKeypoints_2);


    drawKeypoints(img_1, surfKeypoints_1, surfImg_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(img_2, surfKeypoints_2, surfImg_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    imshow("Surf", surfImg_keypoints_1);
    waitKey();
    imshow(" Surf", surfImg_keypoints_2);
    waitKey();




    return 0 ;

}


void readme()
{ std::cout << " Usage: ./SURF and SIFT! <img1> <img2>" << std::endl; }