
#include "Image.h"
#include "EdgeDetector.h"
#include "Histogram.h"
using namespace std;

#include <iostream>
#include <algorithm>

// Supports only gray scale or 3 channel images for now..


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
            cout << "WTF?" << endl;
        }
    }

    return filePresent;
}

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
/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 11)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <kneepointsHistLog.txt> <Sobelbeforethreshold.raw> <xgradient.raw> <ygradinet.raw> <Sobelafter threshold.raw> <LogOutput.raw>" << endl;
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

        //display_help(*img);
        EdgeDetector* detector = new EdgeDetector(*img, false);

        //separate to individual channels and apply sobel detector

        Image* singleChannel = new Image(img->width, img->height, 1);
        singleChannel->allocate();
        ctog_luminosity(*img,singleChannel);

       // display_help(*singleChannel);
       // detector->all_channel(redChannel, greenChannel,blueChannel );



        //output channels for after sobel detector

        Image* singleSobelChannel = new Image(img->width, img->height, 1);

        singleSobelChannel->allocate();

        Image* xSobelChannel = new Image(img->width, img->height, 1);

        xSobelChannel->allocate();

        Image* ySobelChannel = new Image(img->width, img->height, 1);

        ySobelChannel->allocate();


        //apply sobel detector

        detector->texture_analysis(singleChannel,singleSobelChannel,xSobelChannel,ySobelChannel );

        //display_help(*singleSobelChannel);
        //display_help(*xSobelChannel);
        //display_help(*ySobelChannel);

        singleSobelChannel->print_image(argv[6], singleSobelChannel);
        xSobelChannel->print_image(argv[7], xSobelChannel);
        ySobelChannel->print_image(argv[8], ySobelChannel);




        //now get the cdf and set the thrshold

        Histogram* histogram = new Histogram(*singleSobelChannel, false);
        Image* equalizedImg = new Image(img->width, img->height, 1);
        equalizedImg->name = ( " - Sobel Ouput");
        equalizedImg->allocate();

        histogram->equalize_histogram(equalizedImg);

        //display_help(*equalizedImg);
        equalizedImg->print_image(argv[9], equalizedImg);






        /////////////////////////////////////////////////////////LOG//////////////////////////////////////////////////////


        //separate the three channels
         EdgeDetector* Log = new EdgeDetector(*img, false);



        //convert to grayscale
        Image* singleChannelLog = new Image(img->width, img->height, 1);
        singleChannelLog->allocate();
        ctog_luminosity(*img,singleChannelLog);


        //find nearest neighbors and calculate the LOG value
        Image* outputChannelLog = new Image(img->width, img->height, 1);
        outputChannelLog->allocate();

        Log->log_analysis(singleChannelLog,outputChannelLog,argv[5]);
       // display_help(*outputChannelLog);
        outputChannelLog->print_image(argv[10], outputChannelLog);




    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

