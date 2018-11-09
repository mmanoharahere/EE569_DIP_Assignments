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





void SingleChannel::median_filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel){
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
                      //  std::cout<<" in col "<< searchRow <<searchCol<< searchPixels[searchPixelsCount] << " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        searchPixelsCount++;
                     //   std::cout<< " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        continue;
                    }

                    if (searchRow < 0 || searchRow >= reducedChannel->height) {
                        searchPixels[searchPixelsCount] = -1;
                      //  std::cout<<" in row "<< searchRow <<searchCol<< searchPixels[searchPixelsCount] << " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        searchPixelsCount++;
                       // std::cout<< " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        continue;
                    }

                    int searchPixelLocation = (searchRow * reducedChannel->width) + searchCol;
                    searchPixels[searchPixelsCount] = (int) reducedChannel->data[searchPixelLocation];
                    searchPixelsCount++;
                  //  std::cout<< " out searchPixelsCount++ "<< searchRow <<searchCol<<searchPixelsCount<<std::endl;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    //                   std::cout<< searchCol << "," << searchRow << "(" << searchPixelsCount << ")" << searchPixels[searchPixelsCount] << std::endl;
                    searchPixelsCount++;
                  //  std::cout<< " out searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                }
            }

            int medianIntensityNeighborhood = calc_median_intensity(searchPixels, searchPixelsSize);
            assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;

        }
    }

}

int SingleChannel::calc_median_intensity(int* searchPixels, int searchPixelsSize) {
    // We know that -1 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it

    int freq[256];
    memset(freq, 0, 256 * sizeof(int));

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue >= 0) {
            freq[currPixelValue]++;
          //  std::cout<<freq[currPixelValue]<<" freq[currPixelValue] " << currPixelValue <<" currPixelValue "<<std::endl;
        }
    }

    int medianIntensity = -100; // Initialize to a minimum

    int sumFrequencies = 0;
   // memset(sumFrequencies, 0, 256 * sizeof(int));

    int medianIndex = 0;

    for (int i = 0; i < 256; i++) {
        sumFrequencies += freq[i];
      //  std::cout<<sumFrequencies<<" sumFrequencies[i] "<<i <<" i "<<std::endl;
       // medianIndex = sumFrequencies[i];
       // std::cout<<medianIndex<<" medianIndex "<<std::endl;
    }
   int finalMedianIndex = (sumFrequencies + 1) / 2;
    int sumfreq = 0;
    for (int i = 0; i < 256; i++) {

        sumfreq += freq[i];
       // std::cout<<" sumfreq" << sumfreq<<std::endl;
        if (sumfreq >= finalMedianIndex) {
            medianIntensity = i;
            break;
        }

    }
  //  std::cout<<medianIntensity<<" medianIntensity " <<std::endl;
    return medianIntensity;
}


void SingleChannel::low_pass_filter_nn_assign(Image* reducedChannel, int neighborhood, Image* assignedChannel){
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
                        //  std::cout<<" in col "<< searchRow <<searchCol<< searchPixels[searchPixelsCount] << " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        searchPixelsCount++;
                        //   std::cout<< " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        continue;
                    }

                    if (searchRow < 0 || searchRow >= reducedChannel->height) {
                        searchPixels[searchPixelsCount] = -1;
                        //  std::cout<<" in row "<< searchRow <<searchCol<< searchPixels[searchPixelsCount] << " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        searchPixelsCount++;
                        // std::cout<< " searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                        continue;
                    }

                    int searchPixelLocation = (searchRow * reducedChannel->width) + searchCol;
                    searchPixels[searchPixelsCount] = (int) reducedChannel->data[searchPixelLocation];
                    searchPixelsCount++;
                    //  std::cout<< " out searchPixelsCount++ "<< searchRow <<searchCol<<searchPixelsCount<<std::endl;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    //                   std::cout<< searchCol << "," << searchRow << "(" << searchPixelsCount << ")" << searchPixels[searchPixelsCount] << std::endl;
                    searchPixelsCount++;
                    //  std::cout<< " out searchPixelsCount++ "<<searchPixelsCount<<std::endl;
                }
            }

            int medianIntensityNeighborhood = low_pass_filter(searchPixels, searchPixelsSize);
            assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;

        }
    }

}


int SingleChannel::low_pass_filter(int* searchPixels, int searchPixelsSize) {
    // We know that -1 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it

    int freq[256];
    memset(freq, 0, 256 * sizeof(int));

    int sumOfIntensities = 0;

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue >= 0) {
            freq[currPixelValue]++;
            sumOfIntensities += currPixelValue;
           //  std::cout<<freq[currPixelValue]<<" freq[currPixelValue] " << currPixelValue <<" currPixelValue "<<std::endl;
        }
    }

     int sumFrequencies = 0;
    // memset(sumFrequencies, 0, 256 * sizeof(int));


    for (int i = 0; i < 256; i++) {
        sumFrequencies += freq[i];
        //
        // std::cout<<sumFrequencies<<" sumFrequencies "<<sumOfIntensities<<" sumOfIntensities "<<i <<" i "<<std::endl;
        // medianIndex = sumFrequencies[i];
        // std::cout<<medianIndex<<" medianIndex "<<std::endl;
    }
    int finalMeanValue = (sumOfIntensities) / sumFrequencies;

    //  std::cout<<medianIntensity<<" medianIntensity " <<std::endl;
    return finalMeanValue;
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

double SingleChannel::psnr_channel(Image * inputChannel, Image *  cleanChannel, double result){

    int maximum = 255;
    double totalSum = 0;
    int imgSize = inputChannel->height * inputChannel->width * inputChannel->numChannels;
    double MSE = 0;


    for (int row = 0; row < inputChannel->height; row++) {
        for (int col = 0; col < inputChannel->width; col++) {
            int currentPixel = (row * inputChannel->width) + col;
            if(inputChannel->numChannels == 1) {
            int currentLocation = currentPixel * 1;

            unsigned char cleanValue = cleanChannel->data[currentLocation];
            unsigned char filteredValue = inputChannel->data[currentLocation];

            int difference = filteredValue - cleanValue;
            double sqrtDiff = (pow(difference, 2));

            totalSum +=  sqrtDiff;
            }

            if(inputChannel->numChannels == 3) {
                int currentLocation = currentPixel * 3;

                unsigned char cleanValueRed = cleanChannel->data[currentLocation + 0];
                unsigned char filteredValueRed = inputChannel->data[currentLocation + 0];

                unsigned char cleanValueGreen = cleanChannel->data[currentLocation + 1];
                unsigned char filteredValueGreen = inputChannel->data[currentLocation + 1];

                unsigned char cleanValueBlue = cleanChannel->data[currentLocation + 2];
                unsigned char filteredValueBlue = inputChannel->data[currentLocation + 2];

                int differenceRed = filteredValueRed - cleanValueRed;
                int differenceGreen = filteredValueGreen - cleanValueGreen;
                int differenceBlue = filteredValueBlue - cleanValueBlue;

                double sqrtDiffRed = (pow(differenceRed, 2));
                double sqrtDiffBlue = (pow(differenceBlue, 2));
                double sqrtDiffGreen = (pow(differenceGreen, 2));

                double sqrtDiff = sqrtDiffRed + sqrtDiffBlue + sqrtDiffGreen;
                totalSum +=  sqrtDiff;
            }

        }
    }

    double intermediateValue = (1 / (double)imgSize);
    MSE = (double) intermediateValue * totalSum;
    result = 10 * log10((pow(maximum, 2)) / ( 1.0 * MSE));
    return result;


}





