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

    void median_filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel);

    void low_pass_filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel);

    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);
    double psnr_channel(Image * inputChannel, Image *  cleanChannel, double result);

  //  void low_pass_filter(Image* singleChannelImg, Image* filteredChannelImg, Image* maskImage);

private:

    const Image* inImg;
    Image* singlePaddedChannelData;


    bool printDebug ;
    int windowSize;
    int numPadding;
    unsigned char noisyMeanData;

    int calc_median_intensity(int* searchPixels, int searchPixelsSize);
    int low_pass_filter(int* searchPixels, int searchPixelsSize);
};


#endif //IMAGEPROCESSINGHW_1CMD_NOISE_SINGLECHANNEL_H
