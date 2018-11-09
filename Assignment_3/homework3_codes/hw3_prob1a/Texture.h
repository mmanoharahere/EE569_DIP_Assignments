//
// Created by Manasa Manohara on 3/13/18.
//

#ifndef HW3_PROB1A_TEXTURE_H
#define HW3_PROB1A_TEXTURE_H
#include "Image.h"

class Texture {
public:

    Texture(Image& image, bool printDebug);
    ~Texture();

    void tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize);
    void texture_analysis(Image* inputImg, double* imgFilter);
    void removeDC(Image* inputImg, double* reducedDCImg);

    void kmeans( double** featureArray, int classes, double** meansArray);
    double nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, Image* assignedChannel, double* currentFilter);
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
#endif //HW3_PROB1A_TEXTURE_H
