
#ifndef IMAGEPROCESSINGXHW_1CMD_NOISE_HISTOGRAM_H
#define IMAGEPROCESSINGXHW_1CMD_NOISE_HISTOGRAM_H

#include "Image.h"

class Histogram {
public:

    Histogram(Image& image, bool printDebug);
    ~Histogram();
    void calc_histogram();
    void print_histogram(const std::string &fileName);
    void equalize_histogram(Image* outImg);

private:


    void calc_probabilities();
    void hist_image(Image* outImg);


    const Image* inImg;
    bool printDebug;

    unsigned int* frequencyRed;
    unsigned int* frequencyGreen;
    unsigned int* frequencyBlue;

    float* probabilitiesRed;
    float* probabilitiesGreen;
    float* probabilitiesBlue;

    float* cdfRed;
    float* cdfGreen;
    float* cdfBlue;

    double* newPixelValRed;
    double* newPixelValGreen;
    double* newPixelValBlue;
};


#endif //IMAGEPROCESSINGXHW_1CMD_NOISE_HISTOGRAM_H
