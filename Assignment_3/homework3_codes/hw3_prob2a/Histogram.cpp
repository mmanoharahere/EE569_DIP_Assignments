//
// Created by Manasa Manohara on 3/18/18.
//


#include "Histogram.h"
#include <iostream>
#include<math.h>
#include<memory.h>

Histogram::Histogram(Image& image, bool printDebug) {

    this->inImg = &image;
    this->printDebug = printDebug;

    this->frequency = new unsigned int[256]; // Frequencies of pixel intensities, for pixel intensity 35, increment frequency[35];
    

    // Initialize all frequencies to zero
    memset(this->frequency, 0, 256 * sizeof(unsigned int));
    

    this->probabilities = new float[256]; // Frequency by NoOfPixels
    memset(this->probabilities, 0, 256 * sizeof(float)); //set all probabilities to zero

   
    this->cdf = new float[256]; // Frequency by NoOfPixels
    memset(this->cdf, 0, 256 * sizeof(float)); //set all cdf to zero

   
    this->newPixelVal = new double[256];
    memset(this->newPixelVal, 0, 256 * sizeof(double));

    


}

Histogram::~Histogram() {
    delete[] this->frequency;
    
    delete[] this->probabilities;
    
    delete[] this->cdf;
    
    delete[] this->newPixelVal;
   
}

void Histogram::calc_histogram() {
    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber = (col + (row * this->inImg->width));
            int currentPixelLocation = currentPixelNumber * this->inImg->numChannels;

            unsigned char currentPixelValue = this->inImg->data[currentPixelLocation + 0];
           
            // Update the frequency of the currentPixelValue
            this->frequency[currentPixelValue]++;
            
        }
    }


}

void Histogram::calc_probabilities() {

    for (int currIndex = 0; currIndex < 256; currIndex++){

        unsigned int frequencyValue = this->frequency[currIndex]; //find the frequency of the currentIndex
       
        this->probabilities[currIndex] = ((float) (frequencyValue)) / (this->inImg->width * this->inImg->height);
        
        if (currIndex == 0){
            this->cdf[currIndex] = this->probabilities[currIndex] ;
            
        }
        else{
            this->cdf[currIndex]  = this->cdf[currIndex - 1]  + this->probabilities[currIndex];
           
        }
        this->newPixelVal[currIndex] = this->cdf[currIndex]  * 255.0;
        this->newPixelVal[currIndex] = floor(this->newPixelVal[currIndex]);

        
    }
//TO PLOT THE TRANSFER FUNCTION
     for (int i = 0; i < 256; i++) {
         std::cout<< i<<"    "<<this->cdf[i]<<std::endl;
     }
}

void Histogram::equalize_histogram(Image* outImg) {
    this->calc_histogram();
    this->calc_probabilities();
    this->hist_image(outImg);
}


void Histogram::hist_image(Image* outImg) {

    int imgSize = outImg->width * outImg->height * 1;
    int sortedAddress[imgSize];
    memset(sortedAddress, 0,imgSize * sizeof(int) );

    int newLocation = 0;
    for (int i = 0; i < 256; i++) {
        for (int row = 0; row < outImg->height; row++) {
            for (int col = 0; col < outImg->width; col++) {
                int currentPixelNumber = (col + (row * outImg->width));
                int currentPixelLocation = currentPixelNumber * 1;

                unsigned char currentPixelValue = this->inImg->data[currentPixelLocation + 0];
                if(i == currentPixelValue){
                    sortedAddress[newLocation] = currentPixelLocation;
                    //std::cout<< i<<" , "<<newLocation<< " : "<<sortedAddress[newLocation]<<std::endl;
                    newLocation++;
                }

                //unsigned char outPixelValue = (unsigned char) this->newPixelVal[currentPixelValue];


               // outImg->data[currentPixelLocation + 0] = outPixelValue;

            }
        }
    }

    int threshold = 0.9 * imgSize;

    for(int thresLocation = 0; thresLocation < imgSize; thresLocation++){
        int currentAddress = sortedAddress[thresLocation] ;
        if(thresLocation < threshold){
            outImg->data[currentAddress + 0] = 255;

        }

        else{
            outImg->data[currentAddress + 0] = 0;
        }
    }

}
void Histogram::print_histogram(const std::string &fileName) {
   /* FILE* histfile;
    histfile = fopen(fileName.c_str(), "w");

    if(histfile != nullptr){
        for (int i = 0; i < 256; i++) {
            fprintf(histfile, "%d\t%d\t%d\t%d\n", i, this->frequency[i],this->frequencyGreen[i], this->frequencyBlue[i] );
        }
    }
*/
}

void Histogram::calc_histogram_bucket(Image& inputImg,int imgWidth, int imgHeight, int imgChannels, int numBuckets, Image* reducedChannel) {

    int imgByteSize = imgWidth * imgHeight * imgChannels;
    int newLocation = 0;
    int pixelPerBucket = ceil((float)imgByteSize / numBuckets);


    int** sortedNewAddressAllocated = new int*[imgByteSize];
    for (int i = 0; i < imgByteSize; ++i) {
        sortedNewAddressAllocated[i] = new int[numBuckets];
        memset(sortedNewAddressAllocated[i], 0, numBuckets * sizeof(int));

    }
    int** sortedNewAddress = sortedNewAddressAllocated;


    int frequencies[256];
    memset(frequencies, 0, 256 * sizeof(int));

    for (int i = 0; i < 256; i++) {
        for (int row = 0; row < imgHeight; row++) {
            for (int col = 0; col < imgWidth; col++) {

                int currentPixel = (row * imgWidth) + col;
                int currentLocation = currentPixel * 1;
                int currentData =  inputImg.data[currentLocation];

                if (i == currentData ) {
                    int currentBucket = (newLocation) / pixelPerBucket;
                    sortedNewAddress[newLocation][currentBucket] = currentLocation;
                    newLocation++;
                    frequencies[i]++;

                }

            }
        }

    }
    int myNextBucketCurrentLocation = 0;
    for( int myCurrentBucket = 0; myCurrentBucket < numBuckets; myCurrentBucket ++) {

        int myCurrentNewLocation = myCurrentBucket * pixelPerBucket;
        if(myCurrentBucket == (numBuckets -1)){
            myNextBucketCurrentLocation = imgByteSize;
        }
        else{
            myNextBucketCurrentLocation = (myCurrentBucket + 1) * pixelPerBucket;
        }
        for (int myNewLocation = myCurrentNewLocation ; myNewLocation < myNextBucketCurrentLocation ; myNewLocation++) {
            int currentAddress = sortedNewAddress[myNewLocation][myCurrentBucket];
            reducedChannel->data[currentAddress] = (unsigned char) myCurrentBucket;
        }

    }


    for (int i = 0; i < imgByteSize; ++i) {
        delete sortedNewAddressAllocated[i];
    }
    delete[] sortedNewAddressAllocated;
}

void Histogram:: all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel){

    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber =  (row * this->inImg->width) + col;
            int currentLocation = currentPixelNumber * this->inImg->numChannels;
            int singleChannelCurrentLocation = currentPixelNumber * 1;

            redChannel->data[singleChannelCurrentLocation] = this->inImg->data[currentLocation + 0];
            greenChannel->data[singleChannelCurrentLocation] = this->inImg->data[currentLocation + 1];
            blueChannel->data[singleChannelCurrentLocation] = this->inImg->data[currentLocation + 2];
        }
    }

}
void Histogram::combine_channels(Image * reducedChannel, Image * greenucedChannel, Image * blueucedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight){
    for (int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currentPixel = (row * imgWidth) + col;
            int currentLocation = currentPixel * 1;
            int rgbCurrentLocation = currentPixel * 3;
            reducedAllChannels->data[rgbCurrentLocation + 0] = reducedChannel->data[currentLocation];
            reducedAllChannels->data[rgbCurrentLocation + 1] = greenucedChannel->data[currentLocation];
            reducedAllChannels->data[rgbCurrentLocation + 2] = blueucedChannel->data[currentLocation];

        }
    }

}
