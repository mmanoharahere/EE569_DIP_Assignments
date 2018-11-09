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


int Morph::eroded_image(int squareElement, int* searchPixels){

    int squareStructureSize = squareElement * squareElement;

    int HitorMiss = 0;
    int isHit = 0;
    for(int searchLocation = 0; searchLocation < squareStructureSize;searchLocation ++ ){
        int currentData = searchPixels[searchLocation];
        if(currentData > 0){
            if(currentData > 127){
                searchPixels[searchLocation] = 0;
            }
            else{
                searchPixels[searchLocation] = 1;
            }
          //  std::cout<<searchLocation<<" , "<<searchPixels[searchLocation]<<" ," <<std::endl;
            if(searchPixels[searchLocation] == 1 ){
                HitorMiss = HitorMiss + 1;
            }
            else{
                HitorMiss = HitorMiss;
            }
            if(HitorMiss == squareStructureSize){
                isHit = isHit + 1;
            }
            else{
                isHit = 0;
            }
        }

    }
return isHit;
}

int Morph::dilated_image(int squareElement, int* searchPixels){

    int squareStructureSize = squareElement * squareElement;
    int isHit = 0;
    for(int searchLocation = 0; searchLocation < squareStructureSize;searchLocation ++ ){
        int currentData = searchPixels[searchLocation];
        if(currentData > 0){
            if(currentData > 127){
                searchPixels[searchLocation] = 0;
            }
            else{
                searchPixels[searchLocation] = 1;
            }
            //  std::cout<<searchLocation<<" , "<<searchPixels[searchLocation]<<" ," <<std::endl;
            if(searchPixels[searchLocation] == 1 ){
                isHit++;
                break;
            }
            else{
                isHit =0;
            }
        }

    }
    return isHit;
}

void Morph::get_dilation(Image* erodedImage, Image* dilatedImage,int dilation ) {
    int searchPixelsSize = dilation * dilation;

    for (int row = 0; row < erodedImage->height; row++) {
        for (int col = 0; col < erodedImage->width; col++) {
            int currPixelLocation = (row * erodedImage->width) + col;
            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));

            get_nearest_neighbor(erodedImage, dilation, row, col, searchPixels);
            int matchHit = 0;
            matchHit = dilated_image(dilation, searchPixels);
            if (matchHit != 0) {
                dilatedImage->data[currPixelLocation] = 255;
            } else {
                dilatedImage->data[currPixelLocation] = 0;
            }

        }

    }
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

void Morph::get_erosion(Image* reducedChannel, Image* outputChannel,Image* erodedImage, Image* dilatedImage, int dilation, int squareElement) {


    int searchPixelsSize = squareElement * squareElement;

    for(int row = 0; row < reducedChannel->height; row++){
        for(int col = 0; col < reducedChannel->width; col++){
            int currPixelLocation = (row * reducedChannel->width) + col;
            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));

             get_nearest_neighbor(reducedChannel,  squareElement, row, col,searchPixels);
            //for(int i = 0; i < searchPixelsSize; i++){
           //     std::cout<<i<<" , "<<searchPixels[i]<<std::endl;
           // }
            int matchHit = 0;
            matchHit =  eroded_image(squareElement, searchPixels);
            if(matchHit!= 0){
                erodedImage->data[currPixelLocation] = 255;
            }
            else{
                erodedImage->data[currPixelLocation] = 0;
            }

            ///Now dilate the image of size 27*27
           // int centerPixel = searchPixelsSize / 2;
            //int centerNormPixelValue = searchPixels[centerPixel];

        }
    }

    get_dilation(erodedImage, dilatedImage,dilation );

}
//////////////////////////////////////////////////STAGE -1//////////////////////////////////////////////////////////

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
    int myblah = 0;
    for (int newrow = 0; newrow < inputChannel->height; newrow++) {
        for (int newCol = 0; newCol < inputChannel->width; newCol++) {
            int newcurrPixelLocation = (newrow * inputChannel->width) + newCol;
            int newCurrData = outputChannel->data[newcurrPixelLocation];
            if(newCurrData > 0){
                myblah++;
              //  std::cout<<"OutputChannel "<< newCurrData<< "  newcurrPixelLocation  "<< newrow<< " , " <<newCol<< " , " << myblah<<std::endl;

            }
        }
        
    }
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////UNIQUE PIECES IN A BOARD/////////////////////////////////////////////////////////////

void Morph::get_connected_components(Image* inputImg, int* connectedComp, Image* outputImg, Image* labelImg, Image* XORImg){

    int current = 0;
    int neighbor = 3;
    int searchPixelsSize = neighbor * neighbor;
    int imgSize = inputImg->height*inputImg->width;

    int** input_4_2DArrayAllocated = new int* [inputImg->height];
    for(int i = 0; i < inputImg->height; i++){
        input_4_2DArrayAllocated[i] = new int[inputImg->width];
        memset(input_4_2DArrayAllocated[i], 0, inputImg->width * sizeof(int));
    }
    int** input_4_2DArray = input_4_2DArrayAllocated;

    int** labelAnsAlloc = new int* [inputImg->height];
    for(int i = 0; i < inputImg->height; i++){
        labelAnsAlloc[i] = new int[inputImg->width];
        memset(labelAnsAlloc[i], 0, inputImg->width * sizeof(int));
    }
    int** labelAns = labelAnsAlloc;

    get_2Darray(inputImg, input_4_2DArray, inputImg->width);


    int parent[imgSize];
    memset(parent, 0, imgSize * sizeof(int));

    numberIslands(labelAns,input_4_2DArray);
    /*for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
        // std::cout<<labelAns[row][col]<< "  ";

        }
        std::cout<<"\n";
    }*/
    int numJigsaws = 0;
    for(int currrow = 0; currrow < inputImg->height; currrow++){
        for(int currcol = 0; currcol < inputImg->width; currcol++){
        int currPixelValue = (currrow * inputImg ->width )    + currcol;
            int currentData = labelAns[currrow][currcol];


            if(numJigsaws < currentData){
                numJigsaws = currentData;
            }

            if(currentData > 0){
                outputImg->data[currPixelValue] = (unsigned char) 255;

            }
            else{
                outputImg->data[currPixelValue] = (unsigned char) 0;

            }

        }
    }
    std::cout<<"Number of Jigsaws : " << numJigsaws <<std::endl;

    //////////////////get the size of the jigsaws/////////////////////////////////////////

    int minColIndex[numJigsaws];
    memset(minColIndex, 0, numJigsaws * sizeof(int));

    int maxColIndex[numJigsaws];
    memset(maxColIndex, 0, numJigsaws * sizeof(int));

    int minRowIndex[numJigsaws];
    memset(minRowIndex, 0, numJigsaws * sizeof(int));


    int maxRowIndex[numJigsaws];
    memset(maxRowIndex, 0, numJigsaws * sizeof(int));

    int jigsawDim = get_size_jigsaw(labelAns, numJigsaws, minColIndex, maxColIndex,minRowIndex,maxRowIndex );


    ///get 1D array so that you can get neighbors

    get_1Darray( labelImg, labelAns, inputImg->height);

    //get the XOR output - this will return us only the holes and protrusions

    get_holes_protrusions(labelImg, XORImg, numJigsaws, minColIndex, minRowIndex, jigsawDim);




}

void Morph::merge(int row, int col, int count, int** inputArray, int** labelAns){
    if(row<0||row>=inImg->height||col<0||col>=inImg->width||inputArray[row][col]!=1)
        return;

    //cout<<answer[i][j]<< " , "<< count<<endl;


    labelAns[row][col]=count;

    if(row-1>=0 && labelAns[row-1][col]!=count)
        merge(row-1, col,count,inputArray,labelAns);

    if(row+1<inImg->height && labelAns[row+1][col]!=count)
        merge(row+1, col,count,inputArray,labelAns);

    if(col-1>=0 && labelAns[row][col-1]!=count)
        merge(row, col-1,count,inputArray,labelAns);

    if(col+1<inImg->width && labelAns[row][col+1]!=count)
        merge(row, col+1,count,inputArray,labelAns);

    if(row+1<inImg->height && labelAns[row+1][col]!=count)
        merge(row+1, col,count,inputArray,labelAns);
    if(col+1<inImg->width  && labelAns[row][col+1]!=count)
        merge(row, col+1,count,inputArray,labelAns);
    if(row+1<row && col-1>=0 && inputArray[row+1][col-1]!=count)
        merge(row+1, col-1,count,inputArray,labelAns);
}

void Morph:: numberIslands(int** labelAns, int** input_4_2DArray){
    int count =0;
    for(int row = 0; row < inImg->height; row++){
        for(int col = 0; col < inImg->width; col++){
            if(input_4_2DArray[row][col] == 1 && labelAns[row][col] == 0){
                count++;
                merge(row,col,count,input_4_2DArray,labelAns);
            }
        }
    }
}



void Morph::get_2Darray(Image* inputArray, int** outputArray, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col <size; col++){
            int currentPixelNumber =  (row * size) + col;
            //  int currentLocation = currentPixelNumber * inputArray->numChannels;
             int currentData = inputArray->data[currentPixelNumber];
            if(currentData > 127){
                outputArray[row][col] = 0;
            }
            else{
                outputArray[row][col] = 1;

            }

        }
    }
}

void Morph::get_2Darray_correct(Image* inputArray, int** outputArray, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col <size; col++){
            int currentPixelNumber =  (row * size) + col;
            //  int currentLocation = currentPixelNumber * inputArray->numChannels;
            int currentData = inputArray->data[currentPixelNumber];
            if(currentData > 127){
                outputArray[row][col] = 1;
            }
            else{
                outputArray[row][col] = 0;

            }

        }
    }
}

void Morph::get_1Darray(Image* outputImg, int** inputArray, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col <size; col++){
            int currentPixelNumber =  (row * size) + col;
            //  int currentLocation = currentPixelNumber * inputArray->numChannels;
            int currentData = inputArray[row][col];
            outputImg->data[currentPixelNumber] = (unsigned char) currentData;

        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////FIND THE MINIMUM COL INDEX////////////////////////////////////////

int Morph::get_size_jigsaw(int **labelAns, int numJigsaws, int* minColIndex, int* maxColIndex, int* minRowIndex, int* maxRowIndex){
int jigsawDim = 0;
    for(int current = 1; current < numJigsaws + 1; current++){
        minColIndex[current-1] = inImg->width;
        maxColIndex[current-1] = 0;
        for(int row =0 ; row < inImg->height; row++){
            for(int col = 0; col < inImg->width; col++){

             //   int currentLocation = (row * inImg->width) + col;
                int currentData = labelAns[row][col];
                if(currentData == current){
                    if(col < minColIndex[current-1]){
                        minColIndex[current-1] = col;
                        minRowIndex[current-1] = row;
                    }
                    if(col > maxColIndex[current-1]){
                        maxColIndex[current-1] = col;
                        maxRowIndex[current-1] = row;

                    }
                }

            }
        }

       // std::cout<< " minColIndex[current] :  "<< minRowIndex[current-1]<< " , "<< minColIndex[current-1]<<  " maxColIndex[current] : "<<maxRowIndex[current-1]<<" ,"<<maxColIndex[current-1]<<std::endl;
    }
    jigsawDim = maxColIndex[1] - minColIndex[1];

    return jigsawDim;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//now XOR to only get back the holes and protrusions

void Morph::get_holes_protrusions(Image* labelImg, Image* outPutImg, int numJigsaws, int* minColIndex, int* minRowIndex, int jigsawSize){
    int maskSize = (jigsawSize + 1) * (jigsawSize +1);

    for(int row = 0; row < inImg->height ; row++){
        for(int col= 0; col < inImg->width; col++){
            int currAddress = ( row * inImg->width) + col;
            int currentLabel = labelImg->data[currAddress];
            int insertValue = ((currentLabel > 0)?255:0);
            outPutImg->data[currAddress] = (unsigned char) insertValue;
        }
    }

    int** holeValuesAlloc = new int* [numJigsaws + 1];
    for(int i = 0; i < numJigsaws + 1; i++){
        holeValuesAlloc[i] = new int[4];
        memset(holeValuesAlloc[i], 0, 4 * sizeof(int));
    }
    int** holeValues = holeValuesAlloc;

    int** protValuesAlloc = new int* [numJigsaws + 1];
    for(int i = 0; i < numJigsaws + 1; i++){
        protValuesAlloc[i] = new int[4];
        memset(protValuesAlloc[i], 0, 4 * sizeof(int));
    }
    int** protValues = protValuesAlloc;


    for(int current =1; current < numJigsaws + 1; current++){
        //it's a square jigsaw
        int XorMask[maskSize];
        //memset(XorMask, 1, maskSize * sizeof(int));
        for(int k = 0; k < maskSize; k++){
            XorMask[k] = current;
        }


        int searchPixels[maskSize];
        memset(searchPixels, 0, maskSize * sizeof(int));
        int middleIndex = (jigsawSize ) / 2;
        int rowIndex = minRowIndex[current-1] + middleIndex;
        int colIndex = minColIndex[current-1] + middleIndex;
        //int currMinRowIndex = minRowIndex[current-1];
        //int currMinColIndex = minColIndex[current-1];


        get_nearest_labels(labelImg, jigsawSize, rowIndex, colIndex, searchPixels, XorMask, outPutImg);

       int** holeProtAllocated = new int* [inImg->height];
        for(int i = 0; i < inImg->height; i++){
            holeProtAllocated[i] = new int[inImg->width];
            memset(holeProtAllocated[i], 0, inImg->width * sizeof(int));
        }
        int** holeProtArray = holeProtAllocated;

        get_2Darray_correct(outPutImg, holeProtArray, inImg->width);

       check_hole_protrusion(holeProtArray, jigsawSize, rowIndex, colIndex, holeValues, protValues, current);


    }
    rotate_jigsaw(holeValues, protValues, numJigsaws);
}


void Morph::get_nearest_labels(Image* reducedChannel, int neighborhood, int row, int col, int* searchPixels, int* XorMask, Image* outPutImg) {
    int searchStep = (neighborhood - 1) / 2 ;

    int currPixelLocation = (row * reducedChannel->width) + col;

    // Nothing to do with image channels because I am working with a single channel image here


    // I have to check for the neighborhood pixels to get the most frequent intensity and assign it to my output

    // Initialize mostFrequentIntensity to the current pixel intensity
    unsigned char mostFrequentIntensity = reducedChannel->data[currPixelLocation];

    // Now go through the neighborhood and grab the most frequent intensity

    int searchRowStart = row - searchStep;
    int searchRowEnd = row + searchStep + 1;

    int searchColStart = col - searchStep;
    int searchColEnd = col + searchStep + 1;

    int searchPixelsCount = 0;
   // int searchPixelsSize = neighborhood * neighborhood;

   /* int searchPixelsAddress[searchPixelsSize];
    memset(searchPixelsAddress, 0, searchPixelsSize * sizeof(int));*/

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

            int internsityValue = reducedChannel->data[searchPixelLocation];
            int xorvalue = XorMask[searchPixelsCount];
            searchPixels[searchPixelsCount] = ((internsityValue == xorvalue)?0:1);
            outPutImg->data[searchPixelLocation] = (unsigned char) ( searchPixels[searchPixelsCount] * 255 );
           // std::cout<<"  searchPixels[searchPixelsCount]  "<<searchPixels[searchPixelsCount]<<std::endl;
            //searchPixelsAddress[searchPixelsCount] = searchPixelLocation;
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


////to check if there is a protrusion or hole - compare with size /2 , if there is a 1 < mid/2 - hole else if a protrusion else neither

void Morph::check_hole_protrusion(int** xorArray, int neighborhood, int row, int col ,int** holeValues, int** protValues,int current) {
    int checkOffset = 5;
    int halfSize = neighborhood / 2;
    int midRow = row;
    int midCol = col;
   // std::cout << "Mid Points: " << midRow << "," << midCol << std::endl;
    int tillTop = midRow-halfSize-checkOffset;
    int tillRight = midCol+halfSize+checkOffset;
    int tillBottom = midRow+halfSize+checkOffset;
    int tillLeft =midCol-halfSize-checkOffset;
        int topTrackCount = 0;

        for(int i = midRow; i > (tillTop); i--) {
            if (xorArray[i][midCol] == 1) {
                if( topTrackCount < halfSize) {
                  //  std::cout << "Top - Hole Found " <<current<<   std::endl;
                    holeValues[current][0] = 1;
                    break;
                }
                else {
                  //  std::cout << "Top - Protrusion Found " <<current<< std::endl;
                    protValues[current][0] = 1;
                    break;
                }
            }
            topTrackCount ++;
        }
        int rightTrackCount = 0;

        for(int j = midCol; j <= (tillRight); j++ ) {
            if (xorArray[midRow][j] == 1) {
                if( rightTrackCount < halfSize) {
                  //  std::cout << "Right - Hole Found "<<current << std::endl;
                    holeValues[current][1] = 1;
                    break;
                }
                else {
                  //  std::cout << "Right - Protrusion Found " <<current<< std::endl;
                    protValues[current][1] = 1;
                    break;
                }
            }
            rightTrackCount ++;
        }
        int bottomTrackCount = 0;

        for(int i = midRow; i < (tillBottom); i++) {

            if (xorArray[i][midCol] == 1) {

                if( bottomTrackCount < halfSize) {
                  //  std::cout << "Bottom - Hole Found " <<current << std::endl;
                    holeValues[current][2] = 1;
                    break;
                }
                else {
                    //std::cout << "Bottom - Protrusion Found "<<current << std::endl;
                    protValues[current][2] = 1;
                    break;
                }
            }
            bottomTrackCount ++;
        }

        int leftTrackCount = 0;

        for(int j = midCol; j > (tillLeft); j--) {
            if (xorArray[midRow][j] == 1) {
                if( leftTrackCount < halfSize) {
                  //  std::cout << "Left - Hole Found "<<current << std::endl;
                    holeValues[current][3] = 1;
                    break;
                }
                else {
                   // std::cout << "Left - Protrusion Found "<<current << std::endl;
                    protValues[current][3] = 1;
                    break;
                }
            }
            leftTrackCount ++;
        }
}

void Morph::rotate_jigsaw(int** holeArray, int** protArray, int numJigsaws){
        int unique = 0;
        int aSize = 4;
        int tempHoles[aSize];
        memset(tempHoles, 0, aSize * sizeof(int));
        int tempProt[aSize];
        memset(tempProt,0, aSize * sizeof(int));
    int totalMatches = 0;
    int totalUnique = 0;
    bool ifVisited[numJigsaws];
    memset(ifVisited, 0, numJigsaws * sizeof(bool));

    for(int current = 1 ; current < numJigsaws + 1; current ++ ){
        bool numoneMatches = false;
        bool numtwoMatches = false;
        bool numthreeMatches = false;
        bool numfourMatches = false;
        bool numfiveMatches = false;





        // check with inputOrientation and then with rotated
        int tempHolesval = holeArray[current][0];
        int tempProtval = protArray[current][0];

        for(int j = 0; j < aSize ; j ++){
            tempHoles[j] = holeArray[current][j];
            tempProt[j] = protArray[current][j];
        }

        for(int num = 0; num < aSize; num++){

            for(int i = 1;i <= aSize -1; i++ ){
                tempHoles[i-1] = tempHoles[i];
                tempProt[i-1] = tempProt[i];
            }
            tempHoles[3] = tempHolesval;
            tempProt[3] = tempProtval;
            for(int present = 1; present < numJigsaws + 1; present++){

                if(current != present){
                    int isoneMatch = 0;
                    int istwoMatch = 0;
                    int isthreeMatch = 0;
                    int isfourMatch = 0;
                    int isfiveMatch = 0;


                   // std::cout<< "current , present"<< current << " , " <<present<<std::endl;

                    for(int j = 0; j < aSize; j++){
                        if(tempProt[j] == protArray[present][j] && tempHoles[j]== holeArray[present][j]){
                            isoneMatch++;
                            if(isoneMatch == 4){
                                /*int firstEle = (current > present)?current:present;
                                if(!ifVisited[current] &&  !ifVisited[present])
                                {
                                    ifVisited[firstEle] = true;
                                }
                                */
                                ifVisited[current] = ! (ifVisited[current]);
                                ifVisited[present] = ! (ifVisited[present]);
                                numoneMatches = true;
                                std::cout<< " They are a match! "<< current<< " == "<< present<<std::endl;
                            }
                        }
                        if(tempProt[j] == protArray[present][aSize - j - 1] && tempHoles[j]== holeArray[present][j]){
                            istwoMatch++;
                            if(istwoMatch == 4){
                               /* int firstEle = (current > present)?current:present;
                                if(!ifVisited[current] &&  !ifVisited[present])
                                {
                                    ifVisited[firstEle] = true;
                                }*/
                                ifVisited[current] = ! (ifVisited[current]);
                                ifVisited[present] = ! (ifVisited[present]);
                                numtwoMatches = true;
                                std::cout<< " They are a match! "<< current<< " == "<< present<<std::endl;
                            }
                        }
                        if(tempProt[j] == protArray[present][j] && tempHoles[j]== holeArray[present][aSize - j - 1]){
                            isthreeMatch++;
                            if(isthreeMatch == 4){
                               /* int firstEle = (current > present)?current:present;
                                if(!ifVisited[current] &&  !ifVisited[present])
                                {
                                    ifVisited[firstEle] = true;
                                }*/
                                ifVisited[current] = ! (ifVisited[current]);
                                ifVisited[present] = ! (ifVisited[present]);
                                numthreeMatches = true;
                                std::cout<< " They are a match! "<< current<< " == "<< present<<std::endl;
                            }
                        }
                        if(tempProt[j] == protArray[present][j] && holeArray[current][j]== holeArray[present][j]){
                            isfourMatch++;
                            if(isfourMatch == 4){
                              /*  int firstEle = (current > present)?current:present;
                                if(!ifVisited[current] &&  !ifVisited[present])
                                {
                                    ifVisited[firstEle] = true;
                                }*/
                                ifVisited[current] = ! (ifVisited[current]);
                                ifVisited[present] = ! (ifVisited[present]);
                                numfourMatches = true;
                                std::cout<< " They are a match! "<< current<< " == "<< present<<std::endl;
                            }
                        }
                        if(protArray[current][j] == protArray[present][j] && tempHoles[j]== holeArray[present][j]){
                            isfiveMatch++;
                            if(isfiveMatch == 4){
                               /* int firstEle = (current > present)?current:present;
                                if(!ifVisited[current] &&  !ifVisited[present])
                                {
                                    ifVisited[firstEle] = true;
                                }*/
                                ifVisited[current] = ! (ifVisited[current]);
                                ifVisited[present] = ! (ifVisited[present]);
                                numfiveMatches = true;
                                std::cout<< " They are a match! "<< current<< " == "<< present<<std::endl;
                            }
                        }

                    }
                }

            }

            /*  if(!numifMatches){
                  std::cout<< " They are unique! "<< current<< std::endl;
              }*/
            tempHolesval = tempHoles[0];
            tempProtval = tempProt[0];

        }
        if(!numoneMatches && !numtwoMatches && !numthreeMatches && !numfourMatches && !numfiveMatches){
            unique++;
            std::cout<<" They are unique! : "<<current<<std::endl;
        }

    }
int count = 0;
for(int boolVal = 1 ; boolVal < numJigsaws + 1; boolVal++){
    if(ifVisited[boolVal]){
        count++;
    }
}
    unique = unique + count;
std::cout<<" Number of Unique elements : " <<unique<<std::endl;


}












