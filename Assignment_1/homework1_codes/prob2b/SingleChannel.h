//
// Created by Manasa Manohara on 1/19/18.
//

#ifndef IMAGEPROCESSINGHW_1CMD_NOISE_SINGLECHANNEL_H
#define IMAGEPROCESSINGHW_1CMD_NOISE_SINGLECHANNEL_H

#include "Image.h"

class SingleChannel {
public:

    SingleChannel(Image& image, bool printDebug);
    ~SingleChannel();
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);
    void calc_histogram(Image& inputImg,int imgWidth, int imgHeight, int imgChannels, int numBuckets, Image* reducedChannel);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);
    void filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel);
private:

    const Image* inImg;
    Image* singlePaddedChannelData;
    int calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize);

    bool printDebug ;
    int windowSize;
    int numPadding;
    unsigned char noisyMeanData;
};


#endif //IMAGEPROCESSINGHW_1CMD_NOISE_SINGLECHANNEL_H
