//
// Created by Manasa Manohara on 2/9/18.
//

#ifndef HW2_PROB1A_SINGLECHANNEL_H
#define HW2_PROB1A_SINGLECHANNEL_H
#include "Image.h"

class SingleChannel {
public:

    SingleChannel(Image& image, bool printDebug);
    ~SingleChannel();
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);
    void get_2Darray(Image* inputChannel, unsigned char** outputArray);
    void convertToCircle(unsigned char** inputArray, Image* outputChannel, Image* reversedChannel, int imgWidth, int imgHeight);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);
private:

    const Image* inImg;
    Image* singlePaddedChannelData;
   // int calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize);

    bool printDebug ;
};

#endif //HW2_PROB1A_SINGLECHANNEL_H
