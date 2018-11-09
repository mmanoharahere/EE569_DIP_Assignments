/* Name : Manasa Manohara
 * USC ID: 372258777
 * USC mail : manasama@usc.edu
 * Date - 03/03/2018*/

#include "Image.h"
#include "CMYKColorSpace.h"
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
            cout << "File and image is Null" << endl;
        }
    }

    return filePresent;
}



/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 7)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Please pass parameters in the following format:" << endl;
        cout << "<program_name> <input_image>.raw  <ImgWidth> <ImgHeight> <BytesPerPixel> <separableerroroutput.raw> <mbvq.raw>" << endl;
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
        img->name = (" - Image"); // Refactor this to be better
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CMYKColorSpace* cmykColorSpace = new CMYKColorSpace(*img, false);
        Image* colorSpaceImg = new Image(img->width, img->height, img->numChannels);
        Image* cChannelImg = new Image(img->width, img->height, 1);
        Image* mChannelImg = new Image(img->width, img->height, 1);
        Image* yChannelImg = new Image(img->width, img->height, 1);

        colorSpaceImg->name = (" - CMYK");
        cChannelImg->name = ( " - C");
        mChannelImg->name = ( " - M");
        yChannelImg->name = ( " - Y");
/////////////////////////////////ALLOCATE///////////////////////////////////////////////////////////////////////////

        colorSpaceImg->allocate();
        cChannelImg->allocate();
        mChannelImg->allocate();
        yChannelImg->allocate();

/////////////convert to cmyk color space///////////////////////////////////////////////////////////////////////////////////////

        cmykColorSpace->CMYK_image(colorSpaceImg, cChannelImg, mChannelImg,yChannelImg, true, true, true, true);

//////////////////////////write to file//////////////////////////////////////////////////////////////////////////////////

       /* colorSpaceImg->print_image(argv[5],colorSpaceImg);
        cChannelImg->print_image(argv[6],cChannelImg);
        mChannelImg->print_image(argv[7],mChannelImg);
        yChannelImg->print_image(argv[8],yChannelImg);*/

        //display_help(*cChannelImg);
        //display_help(*mChannelImg);
        //display_help(*yChannelImg);







        /////////////////////////////////////////////Floyd ///////////////////////////////////////////////////////////////////


        Dithering* c_dithering = new Dithering(*cChannelImg,false);
        ////Error diffusion - FLoyd
        int neighborhood = 3;
        Image* floydCChannel = new Image(cChannelImg->width, cChannelImg->height, 1);
        floydCChannel->allocate();
        floydCChannel->name = ("floydCChannel");
        c_dithering->copy_image(cChannelImg,floydCChannel);
        bool isCStucki = false;
        c_dithering->error_diffusion(isCStucki, floydCChannel, neighborhood);
        //display_help(*floydCChannel);

        Dithering* m_dithering = new Dithering(*mChannelImg,false);
        Image* floydMChannel = new Image(mChannelImg->width, mChannelImg->height, 1);
        floydMChannel->allocate();
        floydMChannel->name = ("floydMChannel");
        m_dithering->copy_image(mChannelImg,floydMChannel);
        bool isMStucki = false;
        m_dithering->error_diffusion(isMStucki, floydMChannel, neighborhood);
        //display_help(*floydMChannel);

        Dithering* y_dithering = new Dithering(*yChannelImg,false);
        Image* floydYChannel = new Image(yChannelImg->width, yChannelImg->height, 1);
        floydYChannel->allocate();
        floydYChannel->name = ("floydYChannel");
        y_dithering->copy_image(yChannelImg,floydYChannel);
        bool isYStucki = false;
        y_dithering->error_diffusion(isYStucki, floydYChannel, neighborhood);
        //display_help(*floydYChannel);


        Image* rgbChannel = new Image(img->width, img->height, 3);
        rgbChannel->allocate();
        rgbChannel->name= ("RGB combined");
        cmykColorSpace->RGB_image(rgbChannel,floydCChannel, floydMChannel, floydYChannel);
       // display_help(*img);
       // display_help(*rgbChannel);


        ////////////////////////////////////////////MBVQ//////////////////////////////////////////////////////////////////////////////

       // display_help(*img);
        Image* quadCMY = new Image(img->width, img->height, 3);
        quadCMY->allocate();
        cmykColorSpace-> decide_quadrant(img, quadCMY);
        quadCMY->name = ("quadCMY");
       // display_help(*quadCMY);


        ////split channels

        Image* quadSpaceImg = new Image(img->width, img->height, img->numChannels);
        Image* cQuadChannelImg = new Image(img->width, img->height, 1);
        Image* mQuadChannelImg = new Image(img->width, img->height, 1);
        Image* yQuadChannelImg = new Image(img->width, img->height, 1);

        quadSpaceImg->name = (" - CMYK");
        cQuadChannelImg->name = ( " - C");
        mQuadChannelImg->name = ( " - M");
        yQuadChannelImg->name = ( " - Y");
/////////////////////////////////ALLOCATE///////////////////////////////////////////////////////////////////////////
        quadSpaceImg->allocate();
        cQuadChannelImg->allocate();
        mQuadChannelImg->allocate();
        yQuadChannelImg->allocate();

        CMYKColorSpace* cmykQuadColorSpace = new CMYKColorSpace(*quadCMY, false);
        cmykQuadColorSpace->all_channel(cQuadChannelImg ,mQuadChannelImg,yQuadChannelImg);
      //  display_help(*cQuadChannelImg);

        ///Dithering

        Dithering* cQ_dithering = new Dithering(*cQuadChannelImg,false);
        ////Error diffusion - FLoyd
        neighborhood = 3;
        Image* floydCQChannel = new Image(cQuadChannelImg->width, cQuadChannelImg->height, 1);
        floydCQChannel->allocate();
        cQ_dithering->copy_image(cQuadChannelImg,floydCQChannel);
        bool isCQStucki = false;
        cQ_dithering->error_diffusion(isCQStucki, floydCQChannel, neighborhood);
       // display_help(*floydCQChannel);

        Dithering* mQ_dithering = new Dithering(*mQuadChannelImg,false);
        Image* floydMQChannel = new Image(mQuadChannelImg->width, mQuadChannelImg->height, 1);
        floydMQChannel->allocate();
        mQ_dithering->copy_image(mQuadChannelImg,floydMQChannel);
        bool isMQStucki = false;
        mQ_dithering->error_diffusion(isMQStucki, floydMQChannel, neighborhood);
       // display_help(*floydMQChannel);

        Dithering* yQ_dithering = new Dithering(*yQuadChannelImg,false);
        Image* floydYQChannel = new Image(yQuadChannelImg->width, yQuadChannelImg->height, 1);
        floydYQChannel->allocate();
        yQ_dithering->copy_image(yQuadChannelImg,floydYQChannel);
        bool isYQStucki = false;
        yQ_dithering->error_diffusion(isYQStucki, floydYQChannel, neighborhood);
       // display_help(*floydYQChannel);


        Image* finalChannel = new Image(img->width, img->height, 3);
        finalChannel->allocate();
        cmykQuadColorSpace->RGB_image(finalChannel,floydCQChannel, floydMQChannel,floydYQChannel);
       // display_help(*finalChannel);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//print images



        rgbChannel->print_image(argv[5],rgbChannel);
        finalChannel->print_image(argv[6],finalChannel);







/////////////////////DEALLOCATE//////////////////////////////////////////////////////////////////////////////////////////////
        colorSpaceImg->deallocate();
        cChannelImg->deallocate();
        mChannelImg->deallocate();
        yChannelImg->deallocate();
        ////////Deallocate
        floydCChannel->deallocate();
        floydMChannel->deallocate();
        floydYChannel->deallocate();
        rgbChannel->deallocate();
        quadCMY->deallocate();




    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img->deallocate();



}

