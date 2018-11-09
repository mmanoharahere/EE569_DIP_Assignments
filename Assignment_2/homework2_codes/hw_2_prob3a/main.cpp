/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/
#include "Image.h"
#include "Morph.h"
using namespace std;
#include <iostream>
#include <algorithm>

// Supports only gray scale or 3 channel images for now..


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
    if (argc != 7)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <Outputfirststage.raw> <OutputSecondstage.raw> " << endl;
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
        int neighborhood = 3;
        Morph *morph = new Morph(*img, false);
        Image *invertedimg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        invertedimg->allocate();

        int previousCount = 0;
        Image *shrink_1_img = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        shrink_1_img->allocate();
        Image *shrink_2_img = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        shrink_2_img->allocate();
        morph->get_stage1(img, shrink_1_img, neighborhood);
        morph->get_stage_2(shrink_1_img,shrink_2_img, img,neighborhood);
        int iterationSize = 12;
        int countArray[iterationSize];
        memset(countArray, 0, iterationSize * sizeof(int));
        int iterationIndex = 0;
       previousCount =  morph->get_size_of_stars(shrink_2_img, iterationIndex, countArray, previousCount);
        for(int i = 1; i < iterationSize; i++){
            morph->get_stage1(shrink_2_img, shrink_1_img, neighborhood);
            morph->get_stage_2(shrink_1_img,shrink_2_img,img,neighborhood);
            previousCount =  morph->get_size_of_stars(shrink_2_img, i, countArray, previousCount);
        }

        morph->get_stars_count(shrink_2_img);

        shrink_1_img->print_image(argv[5],shrink_1_img);
        shrink_2_img->print_image(argv[6],shrink_2_img);
        invertedimg->deallocate();
        shrink_1_img->deallocate();
        shrink_2_img->deallocate();
    }
    else {

        std::cout << "Image is not open!!" << endl;
    }


    img->deallocate();



}

