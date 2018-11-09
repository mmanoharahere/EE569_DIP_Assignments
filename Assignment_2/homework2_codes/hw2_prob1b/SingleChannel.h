//
// Created by Manasa Manohara on 2/10/18.
//

#ifndef HW2_PROB1B_SINGLECHANNEL_H
#define HW2_PROB1B_SINGLECHANNEL_H
#include "Image.h"

class SingleChannel {
public:

    SingleChannel(Image& image, bool printDebug);
    ~SingleChannel();
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);

    void embed_image(Image* inputChannel, Image*outputChannel);
    void homographic_trans(float* H_array, Image* originalImg, Image* outputChannel ,  int imgWidth, int imgHeight,int embedWidth, int embedHeight);
    void bilinear_interpolate(Image* originalImg,Image* outputChannel, float new_y,float new_x, int imgHeight, int imgWidth, int outPixelLocation);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);

private:

    const Image* inImg;
    Image* singlePaddedChannelData;
    // int calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize);

    bool printDebug ;
    int windowSize;
    int numPadding;
    unsigned char noisyMeanData;
};
#endif //HW2_PROB1B_SINGLECHANNEL_H
