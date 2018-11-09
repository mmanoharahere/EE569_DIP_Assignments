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
            cout << "File and Image is null" << endl;
        }
    }

    return filePresent;
}

/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 12)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <noisefreeImage>.raw <filter1Output.raw> <filter2Output.raw> <originalHist.txt> <filter1Histogram.txt> <filter2Histogram.txt> <neighborhood>" << endl;
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

        int windowSize = atoi(argv[11]);
        int numPadding = (windowSize - 1) / 2;


        redChannel->name = (" - Red");
        greenChannel->name = ( " - Green");
        blueChannel->name = ( " - Blue");



        redChannel->allocate();
        greenChannel->allocate();
        blueChannel->allocate();



        singleChannel->all_channel(redChannel,greenChannel,blueChannel);



/////////////////////////MEDIAN FILTER///////////////////////////////////////////////////////////////////////////////////////////////////////////

        Image* redMedianChannel = new Image(img->width, img->height, 1);
        redMedianChannel->allocate();
        redMedianChannel->name = (" - RedMedian");

        Image* greenMedianChannel = new Image(img->width, img->height, 1);
        greenMedianChannel->allocate();
        greenMedianChannel->name = (" - GreenMedian");

        Image* blueMedianChannel = new Image(img->width, img->height, 1);
        blueMedianChannel->allocate();
        blueMedianChannel->name = (" - BlueMedian");

        Image* filteredMedianChannel = new Image(img->width, img->height, 3);
        filteredMedianChannel->allocate();
        filteredMedianChannel->name = (" - filteredMedianChannelMedian");

        singleChannel->median_filter_nn_assign(redChannel, windowSize, redMedianChannel);
        singleChannel->median_filter_nn_assign(greenChannel, windowSize, greenMedianChannel);
        singleChannel->median_filter_nn_assign(blueChannel, windowSize, blueMedianChannel);

        singleChannel->combine_channels(redMedianChannel,greenMedianChannel, blueMedianChannel, filteredMedianChannel, inSizeWidth, inSizeHeight);

        filteredMedianChannel->print_image(argv[6],filteredMedianChannel);


////////////////////////////////LOW PASS FILTERING////////////////////////////////////////////////////////////////////////////////////////////////

        Image* redMeanChannel = new Image(img->width, img->height, 1);
        redMeanChannel->allocate();
        redMeanChannel->name = (" - RedMean");

        Image* greenMeanChannel = new Image(img->width, img->height, 1);
        greenMeanChannel->allocate();
        greenMeanChannel->name = (" - GreenMean");

        Image* blueMeanChannel = new Image(img->width, img->height, 1);
        blueMeanChannel->allocate();
        blueMeanChannel->name = (" - BlueMean");

        Image* filteredMeanChannel = new Image(img->width, img->height, 3);
        filteredMeanChannel->allocate();
        filteredMeanChannel->name = (" - filteredMean");

        singleChannel->low_pass_filter_nn_assign(redMedianChannel, windowSize,redMeanChannel );
        singleChannel->low_pass_filter_nn_assign(greenMedianChannel, windowSize, greenMeanChannel);
        singleChannel->low_pass_filter_nn_assign(blueMedianChannel, windowSize, blueMeanChannel);

        singleChannel->combine_channels(redMedianChannel,greenMedianChannel, blueMeanChannel, filteredMeanChannel, inSizeWidth, inSizeHeight);

        filteredMeanChannel->print_image(argv[7],filteredMeanChannel);


/////////////////////////////////////////PSNR////////////////////////////////////////////////////////////////////////////////////////////////////
        Image *cleanImg = new Image(inSizeWidth, inSizeHeight, inNumChannels);
        cleanImg->allocate();
        load_image_from_file(argv[5], *cleanImg);


        Image* redCleanChannel = new Image(cleanImg->width, cleanImg->height, 1);
        redCleanChannel->allocate();
        redCleanChannel->name = (" - RedClean");

        Image* greenCleanChannel = new Image(cleanImg->width, cleanImg->height, 1);
        greenCleanChannel->allocate();
        greenCleanChannel->name = (" - GreenClean");

        Image* blueCleanChannel = new Image(cleanImg->width, cleanImg->height, 1);
        blueCleanChannel->allocate();
        blueCleanChannel->name = (" - BlueClean");

        SingleChannel* singleChannelClean = new SingleChannel(*cleanImg, false);

        singleChannelClean->all_channel(redCleanChannel,greenCleanChannel,blueCleanChannel);

        double psnr_red = 0;
        double psnr_green = 0;
        double psnr_blue = 0;
        double psnr_all = 0;

        double psnrRed = singleChannelClean->psnr_channel(redMedianChannel ,redCleanChannel, psnr_red );
        double psnrGreen = singleChannelClean->psnr_channel(greenMedianChannel ,greenCleanChannel, psnr_green );
        double psnrBlue = singleChannelClean->psnr_channel(blueMeanChannel ,blueCleanChannel, psnr_blue );
        double psnrAll = singleChannelClean->psnr_channel(filteredMeanChannel ,cleanImg, psnr_all );

        std::cout<<" psnr_red "<<psnrRed<<std::endl;
        std::cout<<" psnr_green "<<psnrGreen<<std::endl;
        std::cout<<" psnr_blue "<<psnrBlue<<std::endl;
        std::cout<<" psnr_all "<<psnrAll<<std::endl;




////////////////////////////PRINT ALL HISTOGRAMS//////////////////////////////////////////////////////////////////////////////////////////////////

        Histogram* noisyHistogram = new Histogram(*img, false);
        noisyHistogram->calc_histogram();
        noisyHistogram->print_histogram(argv[8]);

        Histogram* medianHistogram = new Histogram(*filteredMedianChannel, false);
        medianHistogram->calc_histogram();
        medianHistogram->print_histogram(argv[9]);

        Histogram* meanHistogram = new Histogram(*filteredMeanChannel, false);
        meanHistogram->calc_histogram();
        meanHistogram->print_histogram(argv[10]);


/////////////////////////////////////////////DEALLOCATE//////////////////////////////////////////////////////////////////////////////////////////////////////

        redChannel->deallocate();
        greenChannel->deallocate();
        blueChannel->deallocate();



        redMedianChannel->deallocate();
        greenMedianChannel->deallocate();
        blueMedianChannel->deallocate();
        filteredMedianChannel->deallocate();

        redMeanChannel->deallocate();
        greenMeanChannel->deallocate();
        blueMeanChannel->deallocate();
        filteredMeanChannel->deallocate();


    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

