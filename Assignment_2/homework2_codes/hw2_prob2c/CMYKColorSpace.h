//
// Created by Manasa Manohara on 2/25/18.
//

#ifndef HW2_PROB2C_CMYKCOLORSPACE_H
#define HW2_PROB2C_CMYKCOLORSPACE_H


#include "Image.h"

class CMYKColorSpace {
public:

    CMYKColorSpace(Image& image, bool printDebug);
    ~CMYKColorSpace();
    void CMYK_image(Image* outImg,Image* cChannelImage,Image* mChannelImage,Image* yChannelImage, bool isallChannel, bool isRed, bool isGreen, bool isBlue);
    void RGB_image(Image* outRGBImg,Image* cChannelImage, Image* mChannelImage,Image* yChannelImage);
    void getMinimumDistance(int* CMYValue, int* first, int* sec, int* third,int* fourth, int* minVertex);
    void decide_quadrant(Image* inputChannel, Image* outputChannel );
    void get_minimum_vertex(int* CMYValue, int numQuad,int currentPixel, Image* outputChannel );
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);
private:

    const Image* inImg;
    Image* normalizedImg;
    bool printDebug;
    bool isallChannel;
    bool isRed;
    bool isGreen;
    bool isBlue;
    double* newPixelVal;
};


#endif //HW2_PROB2C_CMYKCOLORSPACE_H
