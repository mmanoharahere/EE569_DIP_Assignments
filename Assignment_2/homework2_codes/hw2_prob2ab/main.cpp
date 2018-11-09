/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/
#include "Image.h"
#include "Dithering.h"
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

    if (argc != 14)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <fixedThreshold.raw> <RandomThreshol.raw> <BayerMatrix2.raw> <BayerMatrix4.raw> <BayerMatrix8.raw> <Floyd.raw> <JJN.raw> <Stucki.raw> <4levels.raw>" << endl;
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
        Image* reducedChannel = new Image(img->width, img->height, 1);
        reducedChannel->allocate();
        Dithering* dithering = new Dithering(*img,false);
        dithering->display_4_levels(img, reducedChannel);
       // display_help(*reducedChannel);


        //fixed thresholding
        Image* fixedThresholdChannel = new Image(img->width, img->height, 1);
        fixedThresholdChannel->allocate();

        dithering->fixed_thresholding(img,fixedThresholdChannel);
        //display_help(*fixedThresholdChannel);

        //random thresholding
        Image* randomThresholdChannel = new Image(img->width, img->height, 1);
        randomThresholdChannel->allocate();
        dithering->random_thresholding(img,randomThresholdChannel);
        //display_help(*randomThresholdChannel);

        //order thresholding
        Image* orderThresholdChannel_4 = new Image(img->width, img->height, 1);
        Image* orderThresholdChannel_8 = new Image(img->width, img->height, 1);
        Image* orderThresholdChannel_2 = new Image(img->width, img->height, 1);
        orderThresholdChannel_2->allocate();
        orderThresholdChannel_4->allocate();
        orderThresholdChannel_8->allocate();
        orderThresholdChannel_4->name = ("4 mask size");
        orderThresholdChannel_8->name = ("8 mask size");
        orderThresholdChannel_2->name = ("2 mask size");


        dithering->order_dithering(img, orderThresholdChannel_4, orderThresholdChannel_8,orderThresholdChannel_2);

      // display_help(*orderThresholdChannel_4);
        //display_help(*orderThresholdChannel_8);
        //display_help(*orderThresholdChannel_2);

        //Deallocation


        ////Error diffusion - FLoyd
        int neighborhood = 3;
        Image* floydChannel = new Image(img->width, img->height, 1);
        floydChannel->allocate();
        dithering->copy_image(img,floydChannel);
        bool isStucki = false;
        dithering->error_diffusion(isStucki, floydChannel, neighborhood);
        floydChannel->name = (" Floyd");
        //display_help(*floydChannel);

        //JJN diffusion

        neighborhood = 5;
        isStucki = false;
        Image* JJNChannel = new Image(img->width, img->height, 1);
        JJNChannel->allocate();
        JJNChannel->name = (" JJNChannel");
        dithering->copy_image(img,JJNChannel);
        dithering->error_diffusion(isStucki, JJNChannel, neighborhood);
        //display_help(*JJNChannel);


        //Stucki Diffusion
        neighborhood = 5;
        isStucki = true;
        Image* StuckiChannel = new Image(img->width, img->height, 1);
        StuckiChannel->allocate();
        StuckiChannel->name = (" StuckiChannel");
        dithering->copy_image(img,StuckiChannel);
        dithering->error_diffusion(isStucki, StuckiChannel, neighborhood);
        //display_help(*StuckiChannel);



        //Display 4 levels



        ///print images
       fixedThresholdChannel->print_image(argv[5],fixedThresholdChannel);
        randomThresholdChannel->print_image(argv[6],randomThresholdChannel);
        orderThresholdChannel_2->print_image(argv[7],orderThresholdChannel_2);
        orderThresholdChannel_4->print_image(argv[8],orderThresholdChannel_4);
        orderThresholdChannel_8->print_image(argv[9],orderThresholdChannel_8);

        floydChannel->print_image(argv[10],floydChannel);
        JJNChannel->print_image(argv[11],JJNChannel);
        StuckiChannel->print_image(argv[12],StuckiChannel);
        reducedChannel->print_image(argv[13],reducedChannel);



        ////////Deallocate

        //assignedChannel->deallocate();
        fixedThresholdChannel->deallocate();
        randomThresholdChannel->deallocate();
        orderThresholdChannel_4->deallocate();
        orderThresholdChannel_8->deallocate();

        floydChannel->deallocate();
        JJNChannel->deallocate();
        StuckiChannel->deallocate();
        reducedChannel->deallocate();
    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

