//
// Created by Manasa Manohara on 3/22/18.
//

#ifndef HW3_PROB1C_TEXTURE_H
#define HW3_PROB1C_TEXTURE_H
#include "Image.h"

class Texture {
public:

    Texture(Image& image, bool printDebug);
    ~Texture();

    void tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize);
    void texture_analysis(Image* inputImg, double* imgFilter,const std::string &fileName,const std::string &fileNamePca,const std::string &fileNameOutput);
    double removeDC(double* searchPixels, int searchPixelsSize);
    void calculate_energy(double** inputChannel, int neighborhood, double** energyArray, int filterNum, int imgSize,int imageWidth, int imageHeight);
    //void reflection_padding(double* singleChannelData , int windowSize, int numPadding, double* singlePaddedChannelData);
    static double calc_energy_pixel(double* searchPixels, int searchPixelsSize, int currentFilter, int imgSize);
    void normalize_feature(double** input, double** output, int imgSize);
    void kmeans(  double** featureArray, int classes,const std::string &fileNameOutput);
    void nn_assign_DC(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood);
    double nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, double** filterD_img, double* currentFilter);

    double laws_filter(double* searchPixels, int searchPixelsSize, double* inputFilter);

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

#endif //HW3_PROB1C_TEXTURE_H
