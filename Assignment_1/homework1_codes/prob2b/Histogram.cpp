
#include "Histogram.h"
#include <iostream>
#include<math.h>

Histogram::Histogram(Image& image, bool printDebug) {

    this->inImg = &image;
    this->printDebug = printDebug;

    this->frequencyRed = new unsigned int[256]; // Frequencies of pixel intensities, for pixel intensity 35, increment frequency[35];
    this->frequencyGreen = new unsigned int[256];
    this->frequencyBlue = new unsigned int[256];

    // Initialize all frequencies to zero
    memset(this->frequencyRed, 0, 256 * sizeof(unsigned int));
    memset(this->frequencyGreen, 0, 256 * sizeof(unsigned int));
    memset(this->frequencyBlue, 0, 256 * sizeof(unsigned int));

    this->probabilitiesRed = new float[256]; // Frequency by NoOfPixels
    memset(this->probabilitiesRed, 0, 256 * sizeof(float)); //set all probabilities to zero

    this->probabilitiesGreen = new float[256]; // Frequency by NoOfPixels
    memset(this->probabilitiesGreen, 0, 256 * sizeof(float)); //set all probabilities to zero

    this->probabilitiesBlue = new float[256]; // Frequency by NoOfPixels
    memset(this->probabilitiesBlue, 0, 256 * sizeof(float)); //set all probabilities to zero

    this->cdfRed = new float[256]; // Frequency by NoOfPixels
    memset(this->cdfRed, 0, 256 * sizeof(float)); //set all cdf to zero

    this->cdfGreen = new float[256]; // Frequency by NoOfPixels
    memset(this->cdfGreen, 0, 256 * sizeof(float)); //set all cdf to zero

    this->cdfBlue = new float[256]; // Frequency by NoOfPixels
    memset(this->cdfBlue, 0, 256 * sizeof(float)); //set all cdf to zero

    this->newPixelValRed = new double[256];
    memset(this->newPixelValRed, 0, 256 * sizeof(double));

    this->newPixelValBlue = new double[256];
    memset(this->newPixelValBlue, 0, 256 * sizeof(double));

    this->newPixelValGreen = new double[256];
    memset(this->newPixelValGreen, 0, 256 * sizeof(double));


}

Histogram::~Histogram() {
    delete[] this->frequencyRed;
    delete[] this->frequencyGreen;
    delete[] this->frequencyBlue;
    delete[] this->probabilitiesRed;
    delete[] this->probabilitiesGreen;
    delete[] this->probabilitiesBlue;
    delete[] this->cdfRed;
    delete[] this->cdfGreen;
    delete[] this->cdfBlue;
    delete[] this->newPixelValRed;
    delete[] this->newPixelValGreen;
    delete[] this->newPixelValBlue;

}

void Histogram::calc_histogram() {
    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber = (col + (row * this->inImg->width));
            int currentPixelLocation = currentPixelNumber * this->inImg->numChannels;

            unsigned char currentPixelValueRed = this->inImg->data[currentPixelLocation + 0];
            unsigned char currentPixelValueGreen = this->inImg->data[currentPixelLocation + 1];
            unsigned char currentPixelValueBlue = this->inImg->data[currentPixelLocation + 2];

            // Update the frequency of the currentPixelValue
            this->frequencyRed[currentPixelValueRed]++;
            this->frequencyGreen[currentPixelValueGreen]++;
            this->frequencyBlue[currentPixelValueBlue]++;
        }
    }
}

void Histogram::calc_probabilities() {

    for (int currIndex = 0; currIndex < 256; currIndex++){

        unsigned int frequencyValueRed = this->frequencyRed[currIndex]; //find the frequency of the currentIndex
        unsigned int frequencyValueGreen = this->frequencyGreen[currIndex];
        unsigned int frequencyValueBlue = this->frequencyBlue[currIndex];

        this->probabilitiesRed[currIndex] = ((float) (frequencyValueRed)) / (this->inImg->width * this->inImg->height);
        this->probabilitiesGreen[currIndex] = ((float) (frequencyValueGreen)) / (this->inImg->width * this->inImg->height);
        this->probabilitiesBlue[currIndex] = ((float) (frequencyValueBlue)) / (this->inImg->width * this->inImg->height);

        if (currIndex == 0){
            this->cdfRed[currIndex] = this->probabilitiesRed[currIndex] ;
            this->cdfGreen[currIndex] = this->probabilitiesGreen[currIndex] ;
            this->cdfBlue[currIndex] = this->probabilitiesBlue[currIndex] ;
        }
        else{
            this->cdfRed[currIndex]  = this->cdfRed[currIndex - 1]  + this->probabilitiesRed[currIndex];
            this->cdfGreen[currIndex]  = this->cdfGreen[currIndex - 1]  + this->probabilitiesGreen[currIndex];
            this->cdfBlue[currIndex]  = this->cdfBlue[currIndex - 1]  + this->probabilitiesBlue[currIndex];

        }
        this->newPixelValRed[currIndex] = this->cdfRed[currIndex]  * 255.0;
        this->newPixelValRed[currIndex] = floor(this->newPixelValRed[currIndex]);

        this->newPixelValGreen[currIndex] = this->cdfGreen[currIndex]  * 255.0;
        this->newPixelValGreen[currIndex] = floor(this->newPixelValGreen[currIndex]);

        this->newPixelValBlue[currIndex] = this->cdfBlue[currIndex]  * 255.0;
        this->newPixelValBlue[currIndex] = floor(this->newPixelValBlue[currIndex]);
    }

}

void Histogram::equalize_histogram(Image* outImg) {
    this->calc_histogram();
    this->calc_probabilities();

    if (this->printDebug) {
        this->print_histogram();
    }

    this->hist_image(outImg);
}


void Histogram::hist_image(Image* outImg) {

    for (int row = 0; row < outImg->height; row++) {
        for (int col = 0; col < outImg->width; col++) {
            int currentPixelNumber = (col + (row * outImg->width));
            int currentPixelLocation = currentPixelNumber * outImg->numChannels;

            unsigned char currentPixelValueRed = this->inImg->data[currentPixelLocation + 0];
            unsigned char currentPixelValueGreen = this->inImg->data[currentPixelLocation + 1];
            unsigned char currentPixelValueBlue = this->inImg->data[currentPixelLocation + 2];

            unsigned char outPixelValueRed = (unsigned char) this->newPixelValRed[currentPixelValueRed];
            unsigned char outPixelValueGreen = (unsigned char) this->newPixelValGreen[currentPixelValueGreen];
            unsigned char outPixelValueBlue = (unsigned char) this->newPixelValBlue[currentPixelValueBlue];

            outImg->data[currentPixelLocation + 0] = outPixelValueRed;
            outImg->data[currentPixelLocation + 1] = outPixelValueGreen;
            outImg->data[currentPixelLocation + 2] = outPixelValueBlue;

            /*if (this->printDebug) {
                std::cout << "row:" << row << " col:" << col << " currentPixelLocation: " << currentPixelLocation << " currentPixelValue:" << currentPixelValue << " (int) currentPixelValue:" << (int) currentPixelValue << " outPixelValue:" << outPixelValue << " (int) outPixelValue:" << (int) outPixelValue << std::endl;
            }*/
        }
    }
}
void Histogram::print_histogram() {
    std::cout << "index, freq, prob, cdf, newPixelValue" << std::endl;

   /* for (int i = 0; i < 256; i++) {
        std::cout << i << ", " << this->frequency[i] << ", " << this->probabilities[i] << ", " << this->cdf[i]<< ", " << this->newPixelVal[i]<< std::endl;
    }*/

}

