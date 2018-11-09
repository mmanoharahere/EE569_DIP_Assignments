

//
// Created by Manasa Manohara on 2/17/18.
//


#include "Morph.h"
#include <iostream>
#include<math.h>
#include <stdlib.h>

Morph::Morph(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

Morph::~Morph() {

}


void Morph::get_2Darray(Image* inputImg, unsigned char** outputArray){
    for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col <inputImg->width; col++){
            int currentPixelNumber =  (row * inputImg->width) + col;
            //  int currentLocation = currentPixelNumber * inputArray->numChannels;
            outputArray[row][col] = inputImg->data[currentPixelNumber];
        }
    }
}

void Morph::get_normalized2D(unsigned char** inputArray, float** normalizedInput, int imgWidth, int imgHeight){
    for(int row=0; row < imgHeight; row++){
        for(int col = 0; col < imgWidth; col++){
            if(inputArray[row][col] > 127){
                normalizedInput[row][col] = 1;
            }
            else{
                normalizedInput[row][col] = 0;
            }
        }
    }
}


void Morph::invert_image(Image* inputImg, Image* outputImg){
    for(int row =0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
            int currPixelLocation = (row * inputImg->width) + col;
            int currentData = inputImg->data[currPixelLocation];

            ////check the image intensity value and invert it


            if( currentData > 127){
                outputImg->data[currPixelLocation] = 0;
            }
            else{
                outputImg->data[currPixelLocation] = 255;
            }
        }
    }


}

int Morph::calc_bond(int* searchPixels, int searchPixelsSize, int* searchPixelsAddress){
    int bond_count = 0 ;
    int hit_value = 1;
    for(int current = 0; current < searchPixelsSize; current++){
        int pixelNum = current % 2;
        int middleValue = searchPixelsSize / 2;
        int currentData = searchPixels[current];

        // std::cout<<current<<" , "<< currentData<<" , "<<searchPixels[current]<< " , " << searchPixelsAddress[current]<<std::endl;
        if(currentData>=0){
            if(current != middleValue ){
                if(currentData == 1){
                    bond_count = bond_count + pixelNum + 1;

                }
            }

        }

    }
    //  std::cout<<bond_count<<" bond_count "<<std::endl;
    return bond_count;
}


bool Morph::multiplymasks(int *bondPattern, int *searchPixels, int searchPixelsSize, int neighborhood, int maskSize, int currentBond) {
    // We know that -1 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it


    bool isMatch = false;
    int numMasks = maskSize / searchPixelsSize;


    for (int currentMask = 0;
         currentMask < numMasks * searchPixelsSize; currentMask = currentMask + searchPixelsSize) {

        int bondMaskCount = 0;
        for (int i = 0; i < searchPixelsSize; i++) {
            int currentMaskLocation = currentMask + i;
            int middlePixel = searchPixelsSize / 2;

            int currPixelValue = searchPixels[i];
            if (currPixelValue >= 0) {
                if (i != middlePixel) {
                    int currNormPixelValue = searchPixels[i];
                    int currentMaskValue = bondPattern[currentMaskLocation];

                    //  std::cout << i << ", " << currentMaskLocation << " currNormPixelValue " << currNormPixelValue
                    //         << " currentMaskValue " << currentMaskValue << std::endl;

                    if (currNormPixelValue == 1) {
                        if (currNormPixelValue == currentMaskValue) {
                            bondMaskCount = bondMaskCount + i % 2 + 1;
                            //    std::cout << bondMaskCount << " bondMaskCount " << std::endl;
                        }
                    }

                }
            }
        }

        if (bondMaskCount == currentBond) {
            int patternNum = currentMask / searchPixelsSize;
            //   std::cout<<patternNum<<" patternNum "<<std::endl;
            isMatch = true;
            break;
        } else {
            isMatch = false;

        }
    }
    return isMatch;

}



/////////get nearest neighbors

void Morph::get_neighbors(Image* reducedChannel, Image* outputChannel, int neighborhood) {


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

            int searchPixelsAddress[searchPixelsSize];
            memset(searchPixelsAddress, 0, searchPixelsSize * sizeof(int));

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
                    int internsityValue = reducedChannel->data[searchPixelLocation];
                    if(internsityValue > 127){
                        searchPixels[searchPixelsCount] = 1;
                    }

                    else{
                        searchPixels[searchPixelsCount] = 0;
                    }

                    searchPixelsAddress[searchPixelsCount] = searchPixelLocation;
                    searchPixelsCount++;

                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    searchPixelsCount++;

                }
            }

            int centerPixel = searchPixelsSize / 2;
            int centerNormPixelValue = searchPixels[centerPixel];
            if(centerNormPixelValue == 1){
                int currentBond = calc_bond(searchPixels, searchPixelsSize,searchPixelsAddress);
                bool isHit = false;
                //  get_stage_1(currentBond, currPixelLocation,outputChannel,searchPixels,searchPixelsSize,neighborhood);


                /// choose the appropriate mask for the bond size and check if they match

                if(currentBond == 4){
                    int maskSize = 72;
                    isHit =  multiplymasks(stk_bond4,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    //reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                }

                else if(currentBond == 6){
                    int maskSize = 72;
                    isHit =  multiplymasks(stk_bond6,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);

                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }
                else if(currentBond == 7){
                    int maskSize = 36;
                    isHit =  multiplymasks(stk_bond7,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }
                else if(currentBond == 8){
                    int maskSize = 36;
                    isHit =  multiplymasks(stk_bond8,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }
                else  if(currentBond == 9){
                    int maskSize = 72;
                    isHit =  multiplymasks(stk_bond9,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }
                else if(currentBond == 10){
                    int maskSize = 36;
                    isHit =  multiplymasks(stk_bond10,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }

                else  if(currentBond == 11){
                    int maskSize = 36;
                    isHit =  multiplymasks(k_bond11,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
                    if(isHit){
                        outputChannel->data[currPixelLocation] = 255;
                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }
                    // reduceLayer(searchPixelsAddress,searchPixels,searchPixelsSize,outputChannel,isHit);
                }
                else{


                    outputChannel->data[currPixelLocation] = 0;

                }
            }
            else{
                outputChannel->data[currPixelLocation] = 0;

            }

        }
    }

}


//////////////////////////////////////////STAGE - 2//////////////////////////////////////////////////////////////////

void Morph::get_stage_2(Image* inputChannel, Image* outputChannel, Image* firstStageInput, int neighborhood) {

    int searchStep = (neighborhood - 1) / 2;

    for (int row = 0; row < inputChannel->height; row++) {
        for (int col = 0; col < inputChannel->width; col++) {
            int currPixelLocation = (row * inputChannel->width) + col;

            // Nothing to do with image channels because I am working with a single channel image here


            // I have to check for the neighborhood pixels to get the most frequent intensity and assign it to my output

            // Initialize mostFrequentIntensity to the current pixel intensity
            //  unsigned char mostFrequentIntensity = inputChannel->data[currPixelLocation];

            // Now go through the neighborhood and grab the most frequent intensity

            int searchRowStart = row - searchStep;
            int searchRowEnd = row + searchStep;

            int searchColStart = col - searchStep;
            int searchColEnd = col + searchStep;

            int searchPixelsCount = 0;
            int searchPixelsSize = neighborhood * neighborhood;

            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));

            int searchPixelsAddress[searchPixelsSize];
            memset(searchPixelsAddress, 0, searchPixelsSize * sizeof(int));

            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    if (searchCol < 0 || searchCol >= inputChannel->width) {
                        searchPixels[searchPixelsCount] = 0;

                        searchPixelsCount++;

                        continue;
                    }

                    if (searchRow < 0 || searchRow >= inputChannel->height) {
                        searchPixels[searchPixelsCount] = 0;

                        searchPixelsCount++;

                        continue;
                    }


                    int searchPixelLocation = (searchRow * inputChannel->width) + searchCol;
                    // searchPixels[searchPixelsCount] = (int) inputChannel->data[searchPixelLocation];
                    int internsityValue = inputChannel->data[searchPixelLocation];
                    if (internsityValue > 127) {
                        searchPixels[searchPixelsCount] = 1;
                    } else {
                        searchPixels[searchPixelsCount] = 0;
                    }

                    searchPixelsAddress[searchPixelsCount] = searchPixelLocation;
                    searchPixelsCount++;

                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    searchPixelsCount++;

                }
            }

            int centerPixel = searchPixelsSize / 2;
            int centerNormPixelValue = searchPixels[centerPixel];


            int hit = 0;

            for(int blah= 0; blah < searchPixelsSize ; blah ++){
                //  std::cout<< blah << "  " << searchPixels[blah]<< " searchPixels " << row<<" , "<<col<<std::endl;
            }

            if(centerNormPixelValue == 0){

                //   std::cout<<hit<< "  Hit in  else "<< row<< " , " <<col <<std::endl;
                int currentValue = firstStageInput->data[currPixelLocation];
                if(currentValue > 127){
                    //originally 255
                    outputChannel->data[currPixelLocation] = (unsigned char)255
                            ;
                    // std::cout<< (int)outputChannel->data[currPixelLocation]<< " CCCHHHHHHEEEECCCKK " << row << " , "<<col<<std::endl;
                }
                else{
                    outputChannel->data[currPixelLocation] = 0;
                    //   std::cout<< (int)outputChannel->data[currPixelLocation]<< " CCCHHHHHHEEEECCCKK " << row << " , "<<col<<std::endl;
                }

            }

            else {

                /////////////////////////////////////////////////////////////////////////////////////////////Spur////////////////////////////////////////////////////////////////////////
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 1) {
                    hit++;
                }
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 1 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 1 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }
                if (searchPixels[0] == 1 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }


                //////////////////////////////////////////////////////////////////////////Single 4 connection/////////////////////////////////////////////////////////////////////////////
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 1 && searchPixels[8] == 0) {
                    hit++;
                }
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }
                if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                    searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }
                if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }
                /////////////////////////////////////////////////////////////////////////////L-corner/////////////////////////////////////////////////////////////////////////////

                if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
                    searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
                    searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
                       searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                       searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
                    hit++;
                }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                       searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
                       searchPixels[6] == 0 && searchPixels[7] == 1 && searchPixels[8] == 0) {
                    hit++;
                }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
                       searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
                       searchPixels[6] == 0 && searchPixels[7] == 1 && searchPixels[8] == 0) {
                    hit++;
                }

                ////////////////////////////////////////////////// corner cluster///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                if (searchPixels[0] == 1 && searchPixels[1] == 1 && searchPixels[3] == 1 && searchPixels[4] == 1) {
                    hit++;
                }
                if (searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[7] == 1 && searchPixels[8] == 1) {
                hit++;
                }

                /////////////////////////////////////////////////Tee Branch///////////////////////////////////////////////////////////

                if (searchPixels[1] == 1 && searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 1) {
                    hit++;
                }
                if (searchPixels[1] == 1 && searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[7] == 1) {
                hit++;
                }
                if (searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[7] == 1) {
                hit++;
                }
                if (searchPixels[1] == 1 && searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[7] == 1) {
                hit++;
                }
            /////////////////////////////////////////////////////////////////////Vee Branch//////////////////////////////////////////////////////////////

                if (searchPixels[0] == 1 &&  searchPixels[2] == 1 && searchPixels[4]==1 &&
                    (searchPixels[6] || searchPixels[7] || searchPixels[8] == 1)) {
                    hit++;
                }
                if (searchPixels[0] == 1 &&  searchPixels[4] == 1 && searchPixels[6]==1 &&
                    (searchPixels[5] || searchPixels[7] || searchPixels[8] == 1)) {
                    hit++;
                }
                if (searchPixels[4] == 1 &&  searchPixels[6] == 1 && searchPixels[8]==1 &&
                    (searchPixels[0] || searchPixels[1] || searchPixels[2] == 1)) {
                    hit++;
                 }
                if (searchPixels[2] == 1 &&  searchPixels[4] == 1 && searchPixels[8]==1 &&
                     (searchPixels[0] || searchPixels[3] || searchPixels[6] == 1)) {
                    hit++;
                }

            ///////////////////////////////////////////////////////Diagonal Branch////////////////////////////////////////////////////////////////////////

                if (searchPixels[1] == 1 && searchPixels[2] == 0 && searchPixels[3] == 0 && searchPixels[4] == 1 &&
                    searchPixels[5] == 1 && searchPixels[6] == 1 && searchPixels[7] == 0) {
                    hit++;
                }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[3] == 1 &&
                       searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[7] == 0 &&
                       searchPixels[8] == 1) {
                    hit++;
                }  if (searchPixels[1] == 0 && searchPixels[2] == 1 && searchPixels[3] == 1 &&
                       searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[6] == 0 &&
                       searchPixels[7] == 1) {
                    hit++;
                }  if (searchPixels[0] == 1 && searchPixels[1] == 0 && searchPixels[3] == 0 &&
                       searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[7] == 1 &&
                       searchPixels[8] == 0) {
                    hit++;
                }

                // if(hit>1){
                //     std::cout<<hit<< "  Hit in  if "<< row<< " , " <<col <<std::endl;
                // }

                if (hit == 0) {
                    //originally zero
                    outputChannel->data[currPixelLocation] = 0;
                    //  std::cout<< (int)outputChannel->data[currPixelLocation]<< " CCCHHHHHHEEEECCCKK not hit " << row << " , "<<col<<std::endl;

                } else {
                    int currentValue = firstStageInput->data[currPixelLocation];
                    if(currentValue > 127){

                        //originally 255
                        outputChannel->data[currPixelLocation] = 255;
                        //   std::cout<< (int)outputChannel->data[currPixelLocation]<< " CCCHHHHHHEEEECCCKK  hit " << row << " , "<<col<<std::endl;

                    }
                    else{

                        //originally 0
                        outputChannel->data[currPixelLocation] = 0;
                        // std::cout<< (int)outputChannel->data[currPixelLocation]<< " CCCHHHHHHEEEECCCKK  hit " << row << " , "<<col<<std::endl;
                    }

                }

                firstStageInput->data[currPixelLocation]=outputChannel->data[currPixelLocation];

            }

        }

    }


    ///////////////////////////////////////////////////////////////////////////
}

















