/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/
#include "Image.h"
#include "Morph.h"
using namespace std;

#include <iostream>
#include <algorithm>


// Image is not a const because I want to write into this object
bool load_image_from_file(const std::string& inFileName, Image& img) {

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
            cout << "File and image null" << endl;
        }
    }

    return filePresent;
}


/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {
    FILE* file = nullptr;
    FILE* histfile = nullptr;
    if (argc != 10)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <Eroded image> <DilatedImage> <ShrinkStage1> <ShrinkStage2> <XOR Image> " << endl;
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
    img->allocate();
    bool imgOpen = load_image_from_file(argv[1], *img);


    if (imgOpen) {
        img->name = argv[1]; // Refactor this to be better

        std::cout<<"hi"<<std::endl;
        //display_help(*img);


        int neighborhood = 27;
        int sqaureSize = 11;
        int compareMask = 3;
        Morph *morph = new Morph(*img, false);
        Image *invertedimg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        invertedimg->allocate();
        morph->invert_image(img, invertedimg);
        //display_help(*invertedimg);
        Image *erodedImage = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        erodedImage->allocate();
        Image *dilatedImage = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        dilatedImage->allocate();


        Image *shrink_1_img = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        shrink_1_img->allocate();
        Image *shrink_2_img = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        shrink_2_img->allocate();

        morph->get_erosion(invertedimg, shrink_1_img, erodedImage, dilatedImage, neighborhood, sqaureSize);

       // display_help(*erodedImage);
        erodedImage->print_image(argv[5],erodedImage);
       // display_help(*dilatedImage);
        dilatedImage->print_image(argv[6],dilatedImage);
        for(int i = 40; i!=0; i--) {
            std:cout<<"i" <<i<<std::endl;
            morph->get_stage1(dilatedImage, shrink_1_img, compareMask);
            //display_help(*shrink_1_img);
            morph->get_stage_2(shrink_1_img, shrink_2_img, dilatedImage, compareMask);
           // display_help(*shrink_2_img);
        }

        shrink_1_img->print_image(argv[7],shrink_1_img);
        shrink_2_img->print_image(argv[8],shrink_2_img);

        int connectedComp[inSizeWidth * inSizeHeight];
        memset(connectedComp,0,inSizeWidth * inSizeHeight*sizeof(int));

        Image *labelledimg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        labelledimg->allocate();

        Image *labelImg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        labelImg->allocate();

        Image *XORImg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        XORImg->allocate();

        morph->get_connected_components(img,connectedComp,labelledimg,labelImg,XORImg);

       // display_help(*XORImg);
        XORImg->print_image(argv[9],XORImg);
      //  display_help(*shrink_1_img);
      //  morph->get_stage_2(shrink_1_img,shrink_2_img, img,neighborhood);
       // display_help(*shrink_2_img);


        /*for(int i = 10; i!=0; i--){
            morph->get_stage1(shrink_2_img, shrink_1_img,dilatedimg, neighborhood,sqaureSize);
            display_help(*shrink_1_img);
            morph->get_stage_2(shrink_1_img,shrink_2_img,img,neighborhood);
            display_help(*shrink_2_img);
        }*/


        invertedimg->deallocate();
        shrink_1_img->deallocate();
        shrink_2_img->deallocate();
        dilatedImage->deallocate();
        labelledimg->deallocate();
        labelImg->deallocate();
        XORImg->deallocate();
    }
    else {

        std::cout << "Image is not open!!" << endl;
    }


    img->deallocate();



}

