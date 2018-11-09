//
// Created by Manasa Manohara on 3/16/18.
//

#ifndef HW3_PROB1B_TEXTURE_H
#define HW3_PROB1B_TEXTURE_H
#include "Image.h"

class Texture {
public:

    Texture(Image& image, bool printDebug);
    ~Texture();
    void tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize);
    void texture_analysis(Image* inputImg, const std::string& fileName);
    double removeDC(double* searchPixels, int searchPixelsSize);
    void calculate_energy(double** inputChannel, int neighborhood, double** energyArray, int filterNum, int imgSize,int imageWidth, int imageHeight);
    static double calc_energy_pixel(double* searchPixels, int searchPixelsSize);
    void kmeans( double** featureArray, int classes,const std::string& fileName);
    void nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, Image* assignedChannel, double* currentFilter, double** filterD_img);
    void nn_assign_DC(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood);

    double laws_filter(double* searchPixels, int searchPixelsSize, double* inputFilter);
private:

    const Image* inImg;
    Image* singlePaddedChannelData;
    // int calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize);

    bool printDebug ;
    int windowSize;
    int numPadding;
    unsigned char noisyMeanData;
};

#endif //HW3_PROB1B_TEXTURE_H
