//
// Created by Manasa Manohara on 2/9/18.
//

#ifndef HW2_PROB2A_DITHERING_H
#define HW2_PROB2A_DITHERING_H
#include <memory.h>
#include "Image.h"
#include <string>

class Dithering {
public:
    Dithering(Image& image, bool printDebug);
    virtual ~Dithering();
    void fixed_thresholding(Image* inputImage, Image* outputImage);
    void random_thresholding(Image* inputImage, Image* outputImage);
    void order_dithering(Image* inputImage, Image* outputImage_4, Image* outputImage_8,Image* outputImage_2);
    void get_2Darray(const int* inputArray, int** outputArray, int size);
    void get_threshold_matrix(int* input, int size, int* output);
    void copy_image(Image* inputImage, Image* outputImage);
    void error_diffusion(bool isStucki, Image* reducedChannel, int neighborhood);
    void display_4_levels(Image* inputChannel, Image* outputChannel);
private:
    void compare_threshold(Image* inputImage,int *threshold_matrix, Image* outputImage,int size);
    void get_bayer_matrix(const int* bayer_init, int* my_bayer_matrix, int matrixSize);
    void get_neighbors( int row, int col, Image* reducedChannel,int* searchPixels,int* searchAddress ,int neighborhood);
    void get_diffused_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels);
    void get_JJN_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels);
    void get_Stucki_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels);
    void get_updated_image(Image* reducedChannel,int* searchPixels,double* diffusedPixels,int* searchAddress, int searchPixelsSize);
    const Image* inImg;
    bool printDebug;
};
#endif //HW2_PROB2A_DITHERING_H
