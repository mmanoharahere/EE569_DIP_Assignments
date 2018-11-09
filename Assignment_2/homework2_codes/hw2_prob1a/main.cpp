/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/

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
            cout << "WTF?" << endl;
        }
    }

    return filePresent;
}


/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 7)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <disc image.raw> <reversedSquare.raw>" << endl;
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
        //ctog_lightness(*img);
        //ctog_average(*img);
        //ctog_luminosity(*img);


        SingleChannel* singleChannel = new SingleChannel(*img, false);
        Image* redChannel = new Image(img->width, img->height, 1);
        Image* greenChannel = new Image(img->width, img->height, 1);
        Image* blueChannel = new Image(img->width, img->height, 1);


        redChannel->allocate();
        greenChannel->allocate();
        blueChannel->allocate();


        singleChannel->all_channel(redChannel,greenChannel,blueChannel);

        //red array 2D allocation
        unsigned char** red2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            red2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(red2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** red2DArray = red2DArrayAllocated;

        //green array 2D allocation
        unsigned char** green2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            green2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(green2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** green2DArray = green2DArrayAllocated;

        //blue array 2D allocation
        unsigned char** blue2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            blue2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(blue2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** blue2DArray = blue2DArrayAllocated;

        //get respective 2D arrays
        singleChannel->get_2Darray(redChannel, red2DArray);
        singleChannel->get_2Darray(greenChannel, green2DArray);
        singleChannel->get_2Darray(blueChannel, blue2DArray);

        Image* redOutputChannel = new Image(img->width, img->height, 1);
        redOutputChannel->allocate();

        Image* greenOutputChannel = new Image(img->width, img->height, 1);
        greenOutputChannel->allocate();

        Image* blueOutputChannel = new Image(img->width, img->height, 1);
        blueOutputChannel->allocate();

        Image* outputChannel = new Image(img->width, img->height, 3);
        outputChannel->allocate();




        //convert it back

        //red array 2D allocation
        unsigned char** reversedRed2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            reversedRed2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(reversedRed2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** reversedRed2DArray = reversedRed2DArrayAllocated;
        //green array 2D allocation
        unsigned char** reversedGreen2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            reversedGreen2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(reversedGreen2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** reversedGreen2DArray = reversedGreen2DArrayAllocated;

        //blue array 2D allocation
        unsigned char** reversedBlue2DArrayAllocated = new unsigned char* [inSizeHeight];
        for(int i = 0; i < inSizeHeight; i++){
            reversedBlue2DArrayAllocated[i] = new unsigned char[inSizeWidth];
            memset(reversedBlue2DArrayAllocated[i], 0, inSizeWidth * sizeof(unsigned char));
        }
        unsigned char** reversedBlue2DArray = reversedBlue2DArrayAllocated;


        // get 2D arrays for the signle channels
        singleChannel->get_2Darray(redOutputChannel, reversedRed2DArray);
        singleChannel->get_2Darray(greenOutputChannel, reversedGreen2DArray);
        singleChannel->get_2Darray(blueOutputChannel, reversedBlue2DArray);

        //get individual channels

        SingleChannel* singleChannelReverse = new SingleChannel(*outputChannel, false);
        Image* redSquareChannel = new Image(img->width, img->height, 1);
        Image* greenSquareChannel = new Image(img->width, img->height, 1);
        Image* blueSquareChannel = new Image(img->width, img->height, 1);
        Image* allSquareChannel = new Image(img->width, img->height, 3);

        //Allocate memory for them
        redSquareChannel->allocate();
        greenSquareChannel->allocate();
        blueSquareChannel->allocate();
        allSquareChannel->allocate();

        //convert to square

        singleChannel->convertToCircle(red2DArray, redOutputChannel,redSquareChannel, inSizeWidth, inSizeHeight);
        singleChannel->convertToCircle(green2DArray, greenOutputChannel,greenSquareChannel, inSizeWidth, inSizeHeight);
        singleChannel->convertToCircle(blue2DArray, blueOutputChannel,blueSquareChannel, inSizeWidth, inSizeHeight);
        singleChannel->combine_channels(redOutputChannel, greenOutputChannel, blueOutputChannel, outputChannel, inSizeWidth, inSizeHeight);
        singleChannel->combine_channels(redSquareChannel, greenSquareChannel, blueSquareChannel, allSquareChannel, inSizeWidth, inSizeHeight);
       // display_help(*outputChannel);
        outputChannel->print_image(argv[5],outputChannel);
       // display_help(*allSquareChannel);
        allSquareChannel->print_image(argv[6],allSquareChannel);

        //Deallocation
        redChannel->deallocate();
        greenChannel->deallocate();
        blueChannel->deallocate();

        redOutputChannel->deallocate();
        greenOutputChannel->deallocate();
        blueOutputChannel->deallocate();
        outputChannel->deallocate();

        redSquareChannel->deallocate();
        greenSquareChannel->deallocate();
        blueSquareChannel->deallocate();
        allSquareChannel->deallocate();


    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

