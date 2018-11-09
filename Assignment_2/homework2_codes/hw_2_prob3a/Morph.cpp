//
// Created by Manasa Manohara on 2/18/18.
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

int Morph::calc_bond(int* searchPixels, int searchPixelsSize){
    int bond_count = 0 ;
    int hit_value = 1;
    for(int current = 0; current < searchPixelsSize; current++){
        int pixelNum = current % 2;
        int middleValue = searchPixelsSize / 2;
        int currentData = searchPixels[current];
        if(currentData>=0){
            if(current != middleValue ){
                if(currentData == 1){
                    bond_count = bond_count + pixelNum + 1;

                }
            }

        }

    }
    return bond_count;
}


bool Morph::matchmasks(int *bondPattern, int *searchPixels, int searchPixelsSize, int neighborhood, int maskSize, int currentBond) {
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
                    if (currNormPixelValue == 1) {
                        if (currNormPixelValue == currentMaskValue) {
                            bondMaskCount = bondMaskCount + i % 2 + 1;
                        }
                    }

                }
            }
        }

        if (bondMaskCount == currentBond) {
            isMatch = true;
            break;
        } else {
            isMatch = false;

        }
    }
    return isMatch;

}



///////////////////////////////////////////STAGE - 1///////////////////////////////////////////////////////////////////////

void Morph::get_stage1(Image* reducedChannel, Image* outputChannel, int neighborhood) {
    int searchPixelsSize = neighborhood * neighborhood;
    for(int row = 0; row < reducedChannel->height; row++){
        for(int col = 0; col < reducedChannel->width; col++){
            int currPixelLocation = (row * reducedChannel->width) + col;
            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));
            get_nearest_neighbor(reducedChannel, neighborhood, row, col,searchPixels);
            int centerPixel = searchPixelsSize / 2;
            int centerNormPixelValue = searchPixels[centerPixel];
            if(centerNormPixelValue == 1){
                int currentBond = calc_bond(searchPixels, searchPixelsSize);
                bool isHit = false;
                check_if_hit_stage1(outputChannel,searchPixels,searchPixelsSize,neighborhood,currentBond, currPixelLocation);
            }
            else{
                outputChannel->data[currPixelLocation] = 0;

            }

        }
    }

}


//////////////////////////////////////////STAGE - 2//////////////////////////////////////////////////////////////////

void Morph::get_stage_2(Image* inputChannel, Image* outputChannel, Image* firstStageInput, int neighborhood) {

    int searchPixelsSize = neighborhood * neighborhood;

    for (int row = 0; row < inputChannel->height; row++) {
        for (int col = 0; col < inputChannel->width; col++) {
            int currPixelLocation = (row * inputChannel->width) + col;
            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));
            get_nearest_neighbor(inputChannel, neighborhood, row, col,searchPixels);
            int centerPixel = searchPixelsSize / 2;
            int centerNormPixelValue = searchPixels[centerPixel];
            int hit = 0;
            for(int blah= 0; blah < searchPixelsSize ; blah ++){
            }

            if(centerNormPixelValue == 0){

                int currentValue = firstStageInput->data[currPixelLocation];
                if(currentValue > 127){
                    outputChannel->data[currPixelLocation] = (unsigned char)255;
                }
                else{
                    outputChannel->data[currPixelLocation] = 0;
                }

            }

            else {
                int hit = 0;
                hit = check_if_hit_stage2(searchPixels, hit);
                if (hit == 0) {
                    outputChannel->data[currPixelLocation] = 0;
                } else {
                    int currentValue = firstStageInput->data[currPixelLocation];
                    if(currentValue > 127){
                        outputChannel->data[currPixelLocation] = (unsigned char)255;

                    }
                    else{
                        outputChannel->data[currPixelLocation] = 0;
                    }

                }
                firstStageInput->data[currPixelLocation]=outputChannel->data[currPixelLocation];

            }

        }

    }


    ///////////////////////////////////////////////////////////////////////////

}

void Morph::get_nearest_neighbor(Image* reducedChannel, int neighborhood, int row, int col, int* searchPixels) {
    int searchStep = (neighborhood - 1) / 2;

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

}

void Morph::check_if_hit_stage1(Image* outputChannel,int*searchPixels,int searchPixelsSize,int neighborhood,int currentBond, int currPixelLocation ){
    bool isHit = false;
    if(currentBond == 1){
        int maskSize = 36;
        isHit =  matchmasks(s_bond1,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 2){
        int maskSize = 72;
        isHit =  matchmasks(s_bond2,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 3){
        int maskSize = 72;
        isHit =  matchmasks(s_bond3,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 4){
        int maskSize = 36;
        isHit =  matchmasks(stk_bond4,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 5){
        int maskSize = 72;
        isHit =  matchmasks(st_bond5,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);

        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 6){
        int maskSize = 90;
        isHit =  matchmasks(st_bond6,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);

        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }

    }
    else if(currentBond == 7){
        int maskSize = 36;
        isHit =  matchmasks(stk_bond7,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 8){
        int maskSize = 36;
        isHit =  matchmasks(stk_bond8,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else  if(currentBond == 9){
        int maskSize = 72;
        isHit =  matchmasks(stk_bond9,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else if(currentBond == 10){
        int maskSize = 36;
        isHit =  matchmasks(stk_bond10,searchPixels,searchPixelsSize,neighborhood, maskSize, currentBond);
        if(isHit){
            outputChannel->data[currPixelLocation] = 255;
        }
        else{
            outputChannel->data[currPixelLocation] = 0;
        }
    }
    else{

        outputChannel->data[currPixelLocation] = 0;

    }
}

int Morph:: check_if_hit_stage2(int* searchPixels, int hit){
    /////////////////////////////////////////////////////////////////////////////////////////////Spur////////////////////////////////////////////////////////////////////////
    if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 1 && searchPixels[3] == 0 &&
        searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[6] == 0 && searchPixels[7] == 0 &&
        searchPixels[8] == 0) {
        hit++;
    }
    if (searchPixels[0] == 1 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
        searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
        searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }
    //////////////////////////////////////////////////////////////////////////Single 4 connection/////////////////////////////////////////////////////////////////////////////
    if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 && searchPixels[3] == 0 &&
        searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[6] == 0 && searchPixels[7] == 1 &&
        searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }
    /////////////////////////////////////////////////////////////////////////////L-cluster/////////////////////////////////////////////////////////////////////////////

    if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 1 && searchPixels[3] == 0 &&
        searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[6] == 0 && searchPixels[7] == 0 &&
        searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 1 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 1 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 1 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 1 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 1 && searchPixels[7] == 1 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[6] == 0 && searchPixels[7] == 1 && searchPixels[8] == 1) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 1) {
        hit++;
    }
    //////////////////////////////////////////////////4-connected offset///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 1 && searchPixels[3] == 1 &&
        searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[6] == 0 && searchPixels[7] == 0 &&
        searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 1 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[6] == 0 && searchPixels[7] == 0 && searchPixels[8] == 1) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 1 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[6] == 0 && searchPixels[7] == 1 && searchPixels[8] == 0) {
        hit++;
    }
    //////////////////////////////////////////////////Spur corner cluster///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (searchPixels[0] == 0 && (searchPixels[1] || searchPixels[5] == 1) && searchPixels[2] == 1 &&
        searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[6] == 1 && searchPixels[7] == 0 &&
        searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 1 && (searchPixels[1] || searchPixels[3] == 1) && searchPixels[2] == 0 &&
           searchPixels[4] == 1 && searchPixels[5] == 0 && searchPixels[6] == 0 &&
           searchPixels[7] == 0 && searchPixels[8] == 1) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[2] == 1 &&
           (searchPixels[3] || searchPixels[7] == 1) && searchPixels[4] == 1 &&
           searchPixels[5] == 0 && searchPixels[6] == 1 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 1 && searchPixels[1] == 0 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && (searchPixels[5] || searchPixels[7] == 1) &&
           searchPixels[6] == 0 && searchPixels[8] == 1) {
        hit++;
    }

    ///////////////////////////////////////////////////////////// //Corner cluster/////////////////////////////////////////////////////

    if (searchPixels[0] == 1 && searchPixels[1] == 1 && searchPixels[3] == 1 && searchPixels[4] == 1) {
        hit++;
    }
    /////////////////////////////////////////////////Tee Branch///////////////////////////////////////////////////////////

    if (searchPixels[1] == 1 && searchPixels[2] == 0 && searchPixels[3] == 1 && searchPixels[4] == 1 &&
        searchPixels[5] == 1 && searchPixels[7] == 0 && searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[3] == 1 &&
           searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[6] == 0 &&
           searchPixels[7] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 0 && searchPixels[3] == 1 &&
           searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[6] == 0 &&
           searchPixels[7] == 1) {
        hit++;
    }  if (searchPixels[1] == 0 && searchPixels[2] == 0 && searchPixels[3] == 1 &&
           searchPixels[4] == 1 && searchPixels[5] == 1 && searchPixels[7] == 1 &&
           searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[1] == 1 && searchPixels[3] == 1 && searchPixels[4] == 1 &&
           searchPixels[5] == 0 && searchPixels[6] == 0 && searchPixels[7] == 1 &&
           searchPixels[8] == 0) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
           searchPixels[3] == 1 && searchPixels[4] == 1 && searchPixels[5] == 0 &&
           searchPixels[7] == 1) {
        hit++;
    }  if (searchPixels[0] == 0 && searchPixels[1] == 1 && searchPixels[2] == 0 &&
           searchPixels[3] == 0 && searchPixels[4] == 1 && searchPixels[5] == 1 &&
           searchPixels[7] == 1) {
        hit++;
    }  if (searchPixels[1] == 1 && searchPixels[3] == 0 && searchPixels[4] == 1 &&
           searchPixels[5] == 1 && searchPixels[6] == 0 && searchPixels[7] == 1 &&
           searchPixels[8] == 0) {
        hit++;
    }
    /////////////////////////////////////////////////////////////////////Vee Branch//////////////////////////////////////////////////////////////

    if (searchPixels[0] == 1 && (searchPixels[6] || searchPixels[7] || searchPixels[8] == 1) &&
        searchPixels[2] == 1 && searchPixels[4] == 1) {
        hit++;
    }  if (searchPixels[0] == 1 && (searchPixels[2] || searchPixels[5] || searchPixels[8] == 1) &&
           searchPixels[4] == 1 && searchPixels[6] == 1) {
        hit++;
    }  if ((searchPixels[0] || searchPixels[1] || searchPixels[2] == 1) && searchPixels[4] == 1 &&
           searchPixels[6] == 1 && searchPixels[8] == 1) {
        hit++;
    }  if ((searchPixels[0] || searchPixels[3] || searchPixels[6] == 1) && searchPixels[2] == 1 &&
           searchPixels[4] == 1 && searchPixels[8] == 1) {
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

    return hit;
}

int Morph::get_size_of_stars(Image* inputChannel, int iteration_index, int* countArray, int previousCount){
    int neighborhood = 3;
    int count = 0;
    int searchPixelsSize = neighborhood * neighborhood;
    for(int row = 0; row < inputChannel->height; row++){
        for(int col = 0; col < inputChannel->width; col++){
            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));
            get_nearest_neighbor(inputChannel, neighborhood, row, col,searchPixels);
            if(searchPixels[0]==0 && searchPixels[1]==0 && searchPixels[2]==0 && searchPixels[3]==0 && searchPixels[4]==1 && searchPixels[5]==0 && searchPixels[6]==0 && searchPixels[7]==0 && searchPixels[8]==0)

            {
                count++;

            }
        }
    }
    countArray[iteration_index]=count-previousCount;

    if(count-previousCount!=0)

    {

        std::cout<<"For iteration: "<<iteration_index<<std::endl;;

        std::cout<<"Count is: "<<countArray[iteration_index]<<std::endl;

    }

    previousCount=count;
    return previousCount;

}

void Morph::get_stars_count(Image* inputChannel){
    int myblah = 0;
    for (int newrow = 0; newrow < inputChannel->height; newrow++) {
        for (int newCol = 0; newCol < inputChannel->width; newCol++) {
            int newcurrPixelLocation = (newrow * inputChannel->width) + newCol;
            int newCurrData = inputChannel->data[newcurrPixelLocation];
            if(newCurrData > 0){
                myblah++;
            }

        }

    }
    std::cout<<"Total number of stars : " << myblah<<std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





















