//
// Created by Manasa Manohara on 1/19/18.
//
#include "SingleChannel.h"
#include <iostream>
#include<math.h>


SingleChannel::SingleChannel(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

SingleChannel::~SingleChannel(){
}

void SingleChannel:: all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel){
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

void SingleChannel::calc_histogram(Image& inputImg,int imgWidth, int imgHeight, int imgChannels, int numBuckets, Image* reducedChannel) {
    int imgByteSize = imgWidth * imgHeight * imgChannels;
    int newLocation = 0;
    int pixelPerBucket = (imgByteSize / numBuckets);
    int** sortedNewAddressAllocated = new int*[imgByteSize];
    for (int i = 0; i < imgByteSize; ++i) {
        sortedNewAddressAllocated[i] = new int[numBuckets];
        memset(sortedNewAddressAllocated[i], 0, numBuckets * sizeof(int));

    }
    int** sortedNewAddress = sortedNewAddressAllocated;

    int sumBucket[numBuckets];
    memset(sumBucket, 0, numBuckets * sizeof(int));

    int meanBucket[numBuckets];
    memset(meanBucket, 0, numBuckets * sizeof(int));

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
                    sumBucket[currentBucket] = sumBucket[currentBucket] + i;
                    newLocation++;
                    frequencies[i]++;

                }

            }
        }
    }

    for(int currentNumBucket = 0; currentNumBucket < numBuckets; currentNumBucket++ ){
        meanBucket[currentNumBucket] = sumBucket[currentNumBucket] / pixelPerBucket;

    }
    for( int myCurrentBucket =0; myCurrentBucket < numBuckets; myCurrentBucket ++) {
        int myCurrentNewLocation = myCurrentBucket * pixelPerBucket;
        int myNextBucketCurrentLocation = (myCurrentBucket + 1) * pixelPerBucket;
        for (int myNewLocation = myCurrentNewLocation ; myNewLocation <myNextBucketCurrentLocation ; myNewLocation++) {
            int currentAddress = sortedNewAddress[myNewLocation][myCurrentBucket];
            reducedChannel->data[currentAddress] = (unsigned char)meanBucket[myCurrentBucket];

        }
    }

    for (int i = 0; i < imgByteSize; ++i) {
        delete sortedNewAddressAllocated[i];
    }
    delete[] sortedNewAddressAllocated;
}

void SingleChannel::combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight){
    for (int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currentPixel = (row * imgWidth) + col;
            int currentLocation = currentPixel * 1;
            int rgbCurrentLocation = currentPixel * 3;
            reducedAllChannels->data[rgbCurrentLocation + 0] = redReducedChannel->data[currentLocation];
            reducedAllChannels->data[rgbCurrentLocation + 1] = greenReducedChannel->data[currentLocation];
            reducedAllChannels->data[rgbCurrentLocation + 2] = blueReducedChannel->data[currentLocation];

        }
    }

}



void SingleChannel::filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel){
    int searchStep = (neighborhood - 1)/2;

    for (int row = 0; row < reducedChannel->height; row++) {
        for (int col = 0; col < reducedChannel->width; col++) {
            int currPixelLocation = (row * reducedChannel->width) + col;

            // Nothing to do with image channels because I am working with a single channel image here


            // I have to check for the neighborhood pixels to get the most frequent intensity and assign it to my output

            // Initialize mostFrequentIntensity to the current pixel intensity
            unsigned char mostFrequentIntensity = reducedChannel->data[currPixelLocation];

            // Now go through the neighborhood and grab the most frequent intensity

            int searchRowStart = row - searchStep;
            int searchRowEnd = row + searchStep;

            int searchColStart = col - searchStep;
            int searchColEnd = col + searchStep;

            int searchPixelsCount = 0;
            int searchPixelsSize = neighborhood * neighborhood;

            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));

            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    if (searchCol < 0 || searchCol >= reducedChannel->width) {
                        searchPixels[searchPixelsCount] = -1;

                        searchPixelsCount++;

                        continue;
                    }

                    if (searchRow < 0 || searchRow >= reducedChannel->height) {
                        searchPixels[searchPixelsCount] = -1;

                        searchPixelsCount++;

                        continue;
                    }

                    int searchPixelLocation = (searchRow * reducedChannel->width) + searchCol;
                    searchPixels[searchPixelsCount] = (int) reducedChannel->data[searchPixelLocation];
                    searchPixelsCount++;

                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    searchPixelsCount++;

                }
            }

            int mostFrequentIntensityNeighborhood = calc_most_frequent_intensity(searchPixels, searchPixelsSize);
            assignedChannel->data[currPixelLocation] = (unsigned char) mostFrequentIntensityNeighborhood;

        }
    }

}

int SingleChannel::calc_most_frequent_intensity(int* searchPixels, int searchPixelsSize) {
    // We know that -1 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it

    int freq[256];
    memset(freq, 0, 256 * sizeof(int));

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue >= 0) {
            freq[currPixelValue]++;
        }
    }

    int mostFrequent = -100; // Initialize to a minimum
    int mostFrequentValue = -8; // Initialize to something invalid

    for (int i = 0; i < 256; i++) {
        if (freq[i] >= mostFrequent) {
            mostFrequent = freq[i];
            mostFrequentValue = i;
        }
    }

    return mostFrequentValue;
}


