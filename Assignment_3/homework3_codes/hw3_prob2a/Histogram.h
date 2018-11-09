//
// Created by Manasa Manohara on 3/18/18.
//

#ifndef HW3_PROB2A_HISTOGRAM_H
#define HW3_PROB2A_HISTOGRAM_H
#include "Image.h"

class Histogram {
public:

    Histogram(Image& image, bool printDebug);
    ~Histogram();

    void equalize_histogram(Image* outImg);
    void print_histogram(const std::string &fileName);
    void calc_histogram();
    void calc_probabilities();
    void calc_histogram_bucket(Image& inputImg,int imgWidth, int imgHeight, int imgChannels, int numBuckets, Image* reducedChannel);
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth,int imgHeight);
private:



    void hist_image(Image* outImg);


    const Image* inImg;
    bool printDebug;

    unsigned int* frequency;


    float* probabilities;

    float* cdf;


    double* newPixelVal;

};

#endif //HW3_PROB2A_HISTOGRAM_H
