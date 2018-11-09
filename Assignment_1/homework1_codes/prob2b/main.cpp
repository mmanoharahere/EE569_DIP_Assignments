/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 04/02/2018*/


#include "Image.h"
#include "Histogram.h"
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
            cout << "WTF?" << endl;
        }
    }

    return filePresent;
}



/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 8)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <reduced.raw> <quantized.raw> <int neighborhood>" << endl;
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


            SingleChannel* singleChannel = new SingleChannel(*img, false);
            Image* redChannel = new Image(img->width, img->height, 1);
            Image* greenChannel = new Image(img->width, img->height, 1);
            Image* blueChannel = new Image(img->width, img->height, 1);

            int windowSize = 11;
            int numPadding = (windowSize - 1) / 2;

            Image* redReducedChannel = new Image(img->width, img->height, 1);
            Image* greenReducedChannel = new Image(img->width, img->height, 1);
            Image* blueReducedChannel = new Image(img->width, img->height, 1);
            Image* reducedAllChannels = new Image(img->width, img->height, 3);
            //quantization
            Image* redQuantizedChannel = new Image(img->width, img->height, 1);
            Image* greenQuantizedChannel = new Image(img->width, img->height, 1);
            Image* blueQuantizedChannel = new Image(img->width, img->height, 1);
            Image* quantizedAllChannels = new Image(img->width, img->height, 3);

            redChannel->name = (" - Red");
            greenChannel->name = ( " - Green");
            blueChannel->name = ( " - Blue");
            blueChannel->name = ( " - Blue");

            redQuantizedChannel->name = (" - RedReduced");
            greenQuantizedChannel->name = (" - GreenReduced");
            blueQuantizedChannel->name = (" - BlueReduced");
            quantizedAllChannels->name = (" - AllQReduced");

            redReducedChannel->name = (" - RedReduced");
            greenReducedChannel->name = (" - GreenReduced");
            blueReducedChannel->name = (" - BlueReduced");

            redChannel->allocate();
            greenChannel->allocate();
            blueChannel->allocate();

            redQuantizedChannel->allocate();
            greenQuantizedChannel->allocate();
            blueQuantizedChannel->allocate();
            quantizedAllChannels->allocate();


            redReducedChannel->allocate();
            greenReducedChannel->allocate();
            blueReducedChannel->allocate();

            reducedAllChannels->allocate();
            int initialBucketValues = 4;

            singleChannel->all_channel(redChannel,greenChannel,blueChannel);
            singleChannel->calc_histogram(*redChannel ,inSizeWidth,  inSizeHeight, 1, initialBucketValues,redReducedChannel);
            singleChannel->calc_histogram(*greenChannel ,inSizeWidth,  inSizeHeight, 1, initialBucketValues,greenReducedChannel);
            singleChannel->calc_histogram(*blueChannel ,inSizeWidth,  inSizeHeight, 1, initialBucketValues,blueReducedChannel);

            singleChannel->combine_channels(redReducedChannel,greenReducedChannel,blueReducedChannel,reducedAllChannels, inSizeWidth,  inSizeHeight);

            SingleChannel* singleChannelQuantized = new SingleChannel(*reducedAllChannels, false);
            singleChannelQuantized->all_channel(redQuantizedChannel,greenQuantizedChannel,blueQuantizedChannel);

            int neighborhood = atoi(argv[7]);
            singleChannelQuantized->filter_nn_assign(redReducedChannel, neighborhood, redQuantizedChannel);
            singleChannelQuantized->filter_nn_assign(greenReducedChannel, neighborhood, greenQuantizedChannel);
            singleChannelQuantized->filter_nn_assign(blueReducedChannel, neighborhood, blueQuantizedChannel);

            singleChannelQuantized->combine_channels(redQuantizedChannel,greenQuantizedChannel,blueQuantizedChannel,quantizedAllChannels, inSizeWidth,inSizeHeight);

            reducedAllChannels->print_image(argv[5],reducedAllChannels);
            quantizedAllChannels->print_image(argv[6],quantizedAllChannels);



            redChannel->deallocate();
            greenChannel->deallocate();
            blueChannel->deallocate();
            reducedAllChannels->deallocate();

            redQuantizedChannel->deallocate();
            greenQuantizedChannel->deallocate();
            blueQuantizedChannel->deallocate();
            quantizedAllChannels->deallocate();

            redReducedChannel->deallocate();
            greenReducedChannel->deallocate();
            blueReducedChannel->deallocate();

        }
        else {

            std::cout << "Image is not open!!" << endl;
        }

        img->deallocate();



}

