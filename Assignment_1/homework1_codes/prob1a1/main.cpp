/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 02/04/2018*/


#include "Image.h"
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
            cout << "File and image Null" << endl;
        }
    }

    return filePresent;
}

////////////////LIGHTNESS METHOD////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ctog_lightness(const Image& image, Image* lightImg){


    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            int currentPixelNumber = (row * image.width) + col;
            int currentPixelLocation =  currentPixelNumber * image.numChannels;
            int currentPixelLocationLightImg = currentPixelNumber * 1;
            if(image.numChannels == 3) {

                unsigned char currentMinimum = std::min(std::min(image.data[currentPixelLocation + 0], image.data[currentPixelLocation + 1]), image.data[currentPixelLocation + 2]);
                unsigned char  currentMaximum = std::max(std::max(image.data[currentPixelLocation + 0], image.data[currentPixelLocation + 1]), image.data[currentPixelLocation + 2]);
                lightImg->data[currentPixelLocationLightImg] = (unsigned char) ((currentMinimum + currentMaximum)/2);

            }

        }
    }
}

///////////////////////LUMINOSITY METHOD////////////////////////////////////////////////////////////////////////////////////////////////////////
void ctog_luminosity(const Image& image, Image* luminosityImg){


    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            int currentPixelNumber = (row * image.width) + col;
            int currentPixelLocation =  currentPixelNumber * image.numChannels;
            int currentPixelLocationLightImg = currentPixelNumber * 1;
            if(image.numChannels == 3) {
                double currentValue =  ((0.21 * ((int)image.data[currentPixelLocation + 0] )+ 0.72 * (int)(image.data[currentPixelLocation + 1]) + 0.07 * (int)(image.data[currentPixelLocation + 2])));
                luminosityImg ->data[currentPixelLocationLightImg] = (unsigned char)(currentValue);
            }

        }
    }

}

/////////////////////////////////AVERAGE METHOD/////////////////////////////////////////////////////////////////////////////////////////////////
void ctog_average(const Image& image, Image* averageImg){


    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            int currentPixelNumber = (row * image.width) + col;
            int currentPixelLocation =  currentPixelNumber * image.numChannels;
            int currentPixelLocationLightImg = currentPixelNumber * 1;
            if(image.numChannels == 3) {
                int currentValue =  ((int)image.data[currentPixelLocation + 0] + (int) image.data[currentPixelLocation + 1] + (int) image.data[currentPixelLocation + 2]);
                averageImg->data[currentPixelLocationLightImg] = (unsigned char)((currentValue)/3);
            }

        }
    }

}




/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 8)	{	std::cout << "Syntax Error - Incorrect parameter usage" << std::endl;
        std::cout << "Please pass parameters in the correct order:" << std::endl;
        std::cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <lightImg.raw> <averageImg.raw> <LuminosityImg.raw>" << std::endl;
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
        bool imgOpen = load_image_from_file(argv[1], *img); //load img from file

        ///////////////////////////LIGHTNESS METHOD ALLOCATION/////////////////////////////////////////////////

        Image *lightImg = new Image(inSizeWidth, inSizeHeight, 1 );
        lightImg->name = ("LightnessMethod");
        lightImg->allocate();

        ///////////////////////////////LUMINOSITY METHOD ALLOCATION///////////////////////////////////////////

        Image *luminosityImg = new Image(inSizeWidth, inSizeHeight, 1 );
        luminosityImg ->name = ( "luminosityImg");
        luminosityImg ->allocate();

        ////////////////////////////////////////AVERAGE METHOD ALLOCATION/////////////////////////////////////
        Image *averageImg = new Image(inSizeWidth, inSizeHeight, 1 );
        averageImg ->name = ( "AverageMethod");
        averageImg ->allocate();

        //Check if img is open
        if (imgOpen) {
            img->name = ( "Tiffany.raw");

            ///////////////////////CALL THE METHODS////////////////////////////////////////////////////////////

            ctog_lightness(*img, lightImg);
            ctog_average(*img, averageImg);
            ctog_luminosity(*img, luminosityImg);

        }
        else {

            std::cout << "Image is not open!!" << endl;
        }

        ////////////////////////WRITE IMAGE TO FILE////////////////////////////////////////////////////////////

        averageImg->print_image(argv[5], averageImg);
        lightImg->print_image(argv[6], lightImg);
        luminosityImg->print_image(argv[7], luminosityImg);

        ////////////////////////////////////DEALLOCATION////////////////////////////////////////////////////////


        img->deallocate();
        lightImg->deallocate();
        luminosityImg ->deallocate();
        averageImg ->deallocate();

}

