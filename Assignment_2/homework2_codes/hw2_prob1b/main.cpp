/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/
#include<stdio.h>
#include "Image.h"
#include "SingleChannel.h"
using namespace std;

#include <iostream>
#include <algorithm>


// Image is not a const because I want to write into this object
bool load_image_from_file(const string& inFileName, Image& img) {

    bool filePresent = false;

    FILE* file = nullptr;
    file = fopen(inFileName.c_str(), "rb");

    // Check if file is not null AND img's data is not null as well
    if (file != nullptr && img.data != nullptr)
    {
        fread(img.data, sizeof(unsigned char), img.getNumOfBytes(), file);
        fclose(file);
        filePresent = true;
    }
    else {
        if (file == nullptr) {
            cout << "File is not open" << endl;
        }
        else if (img.data == nullptr) {
            cout << "Img data is null" << endl;
        }
        else {
            cout << "File not found?" << endl;
        }
    }

    return filePresent;
}


/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 8)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <left.raw> <right.raw> <Output.raw>" << endl;
        return 0;
    }

    /* initialization of variables */
    int inSizeWidth = atoi(argv[2]); /* number of rows in the image */
    int inSizeHeight = atoi(argv[3]); /* number of columns in the image */
    int inNumChannels = atoi(argv[4]); /* bytes per pixel (RGB = 3, GRAY = 1) */
    // img constructed given dimensions
    // img allocate
    // img load from file
    // do something with img
    // img deallocate

    Image *img = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    Image *leftimg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    Image* rightimg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img->allocate();
    leftimg->allocate();
    rightimg->allocate();
    bool imgOpen = load_image_from_file(argv[1], *img);
    bool imgOpenLeft = load_image_from_file(argv[5], *leftimg);
    bool imgOpenRight = load_image_from_file(argv[6], *rightimg);


    if (imgOpen) {
        img->name = argv[1]; // Refactor this to be better


       // display_help(*leftimg);


        SingleChannel* singleChannel = new SingleChannel(*img, false);
        // embed image in a bigger image
        int embedWidth = 2500;
        int embedHeight  = 1240;
        Image* embedImg = new Image(embedWidth, embedHeight, 3);
        embedImg->allocate();
        singleChannel->embed_image(img, embedImg);


        float rL1 = 100 * 0.007085522931281;
        float rL2 = 100 * -0.001410493508118;
        float rL3 = 100 * -0.767094289296011;
        float rL4 = 100 * -0.000174795607964;
        float rL5 = 100 *  0.005246967576371;
        float rL6 = 100 *  -2.021456226406839;
        float rL7 = 100 *   -0.000000316624742;
        float rL8 = 100 *  -0.000005040236436;
        float
                rL9 = 100 *    0.012032025806476;



        float H_left_array[9] =  {rL1,rL2,rL3,rL4,rL5,rL6,rL7,rL8,rL9};





        double rH1 = 1000 * 0.001110981575213;
        double rH2 = 1000 * 0.000320334477785;
        double rH3 = 1000 * -0.702220261254909;
        double rH4 = 1000 * 0.000001011231121;
        double rH5 = 1000 * 0.001247497630304;
        double rH6 = 1000 *  -1.260823570050726;
        double rH7 = 1000 *   -0.000000041246898;
        double rH8 = 1000 *  0.000000950951874;
        double rH9 = 1000 *   0.000053206924333;






       // double H_right_array[9] =  {rH1,rH2,rH3,rH4,rH5,rH6,rH7,rH8,rH9 };
        float H_right_array[9] = {1.110981575213, 0.320334477785, -702.220261254909, 0.001011231121, 1.247497630304, -1260.823570050726, -0.000041246898, 0.000950951874, 0.053206924333};



        singleChannel->homographic_trans(H_left_array,leftimg , embedImg, inSizeWidth,inSizeHeight, embedWidth, embedHeight);

       singleChannel->homographic_trans(H_right_array, rightimg , embedImg, inSizeWidth,inSizeHeight, embedWidth, embedHeight);


        singleChannel->embed_image(img, embedImg);
     


        embedImg->print_image(argv[7], embedImg);


      //  singleChannel->image_stitching()



        //Deallocation
       leftimg->deallocate();
        rightimg->deallocate();
        embedImg->deallocate();



    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

