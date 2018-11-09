
#include "Image.h"
#include "Texture.h"
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
            cout << "NULLLLL?" << endl;
        }
    }

    return filePresent;
}


/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 6)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <outputImg>.raw" << endl;
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


    Image *img1 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img1->allocate();
    bool imgOpen = load_image_from_file(argv[1], *img1);


    if (imgOpen) {

        // display_help(*img1);
        //initialize means array
        int classWidth = 9;
        int classHeight = 4;

        double** meansArrayAllocated = new double* [classHeight];
        for(int i = 0; i < classHeight; i++){
            meansArrayAllocated[i] = new double[classWidth];
            memset(meansArrayAllocated[i], 0, classWidth * sizeof(double));
        }
        double** meansArray = meansArrayAllocated;

        Texture* texture = new Texture(*img1, false);


        int featureWidth = 9;
        int featureHeight = 12;

        double** featureArrayAllocated = new double* [featureHeight];
        for(int i = 0; i < featureHeight; i++){
            featureArrayAllocated[i] = new double[featureWidth];
            memset(featureArrayAllocated[i], 0, featureWidth * sizeof(double));
        }
        double** featureArray = featureArrayAllocated;

        texture->texture_analysis(img1, argv[5]);

    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img1->deallocate();
}

