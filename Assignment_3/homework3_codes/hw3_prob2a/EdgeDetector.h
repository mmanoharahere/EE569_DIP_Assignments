//
// Created by Manasa Manohara on 3/14/18.
//

#ifndef HW3_PROB2A_EDGEDETECTOR_H
#define HW3_PROB2A_EDGEDETECTOR_H
#include "Image.h"

class EdgeDetector {
public:

    EdgeDetector(Image& image, bool printDebug);
    ~EdgeDetector();
   // void display_help(const Image& image);
    void all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel);
    void get_2Darray(Image* inputChannel, unsigned char** outputArray);
    void print_histogram(const std::string &fileName,int* freq);
    double sobelOperator(const int* searchPixels, int currentLocation);
    double x_gradient(const int* searchPixels,  int currentLocation);
    double y_gradient(const int* searchPixels,  int currentLocation);
    void texture_analysis(Image* inputImg, Image* sobeloutputImg, Image* xgradImg, Image* ygradImg);
    void sobelOutput(const int* searchPixels,  int currentLocation, double maxX, double minX, double maxY, double minY, double maxMAg, double minMag, Image* outputImg, Image* xgradImg, Image* ygradImg);
    void removeDC(Image* inputImg, double* reducedDCImg);
    void nn_assign(Image* reducedChannel, int* searchPixels, int imgWidth, int imgHeight, int neighborhood, int row, int col);
    //int laws_filter(double* searchPixels, int searchPixelsSize, double* inputFilter);
    void combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight);

    void log_analysis(Image* inputImg, Image* outputImage,const std::string &fileName);
    void normalize_values(int* magnitude, int* normArray,int imgWidth, int imgHeight);
    double logOperator(const int* searchPixels, int* outputImg, int currentLocation);
    void calc_freq(int* normArray, int imgHeight, int imgWidth, int* frequency);
    void get_thresholds(int* normalizedArray, Image* thresholdImg, int* thresholdInt, int imageWidth, int imageHeight);
    void zero_crossing(int* searchPixels, Image* outputImg, int currLocation);
    void nn_assign_multiply(int* reducedChannel,int* searchPixels, int imgWidth, int imgHeight, int neighborhood, Image* outputImage);

private:

    const Image* inImg;
    Image* singlePaddedChannelData;
    // int calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize);

    bool printDebug ;
    int windowSize;
    int numPadding;
    unsigned char noisyMeanData;
};
#endif //HW3_PROB2A_EDGEDETECTOR_H
