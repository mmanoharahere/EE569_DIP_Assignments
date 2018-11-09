//
// Created by Manasa Manohara on 1/12/18.
//
#include "CMYKColorSpace.h"
#include <iostream>
#include<math.h>


CMYKColorSpace::CMYKColorSpace(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

CMYKColorSpace::~CMYKColorSpace(){
}

void CMYKColorSpace:: CMYK_image(Image* outCMYImg,Image* cChannelImage, Image* mChannelImage,Image* yChannelImage, bool isallChannel, bool isRed, bool isGreen, bool isBlue){
    this->normalizedImg = new Image(this->inImg->width, this->inImg->height, this->inImg->numChannels);
    this->normalizedImg->allocate();
    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber =  (row * this->inImg->width) + col;
            int currentLocation = currentPixelNumber * this->inImg->numChannels;
            int singleChannelCurrentLocation = currentPixelNumber * 1;

            if(isallChannel){
                unsigned char redValue = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 0])) / 255)) * 255);
                unsigned char greenValue = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 1])) / 255)) * 255);
                unsigned char blueValue = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 2])) / 255)) * 255);
                outCMYImg->data[currentLocation + 0] = redValue;
                outCMYImg->data[currentLocation + 1] = greenValue;
                outCMYImg->data[currentLocation + 2] = blueValue;
            }

            if(isRed){
                cChannelImage->data[singleChannelCurrentLocation] = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 0])) / 255)) * 255);
            }

            if(isGreen){
                mChannelImage->data[singleChannelCurrentLocation] = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 1])) / 255)) * 255);
            }

            if(isBlue){
                yChannelImage->data[singleChannelCurrentLocation] = (unsigned char) ((1 - ((static_cast<float> (this->inImg->data[currentLocation + 2])) / 255)) * 255);
            }

        }
    }

    this->normalizedImg->deallocate();
}


void CMYKColorSpace::RGB_image(Image* outRGBImg,Image* cChannelImage, Image* mChannelImage,Image* yChannelImage){
    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber =  (row * this->inImg->width) + col;
            int currentLocation = currentPixelNumber * this->inImg->numChannels;
            int singleChannelCurrentLocation = currentPixelNumber * 1;

            outRGBImg->data[currentLocation + 0] = (unsigned char) (round) ((1 - ((static_cast<float> (cChannelImage->data[singleChannelCurrentLocation])) / 255)) * 255);
            outRGBImg->data[currentLocation + 1] = (unsigned char) (round)((1 - ((static_cast<float> (mChannelImage->data[singleChannelCurrentLocation])) / 255)) * 255);
            outRGBImg->data[currentLocation + 2] = (unsigned char) (round)((1 - ((static_cast<float> (yChannelImage->data[singleChannelCurrentLocation])) / 255)) * 255);

        }

    }
}

void CMYKColorSpace::decide_quadrant(Image* inputChannel, Image* outputChannel ){

    for(int row = 0; row < inputChannel->height; row++){
        for(int col = 0; col < inputChannel->width; col++){
            int currentPixel = (row * inputChannel->width) + col;
            int currentLocation = currentPixel * inputChannel->numChannels;

            int redValue = inputChannel->data[currentLocation + 0];
            int greenValue = inputChannel->data[currentLocation + 1];
            int blueValue = inputChannel->data[currentLocation + 2];

            int cValue = 255 - redValue;
            int mValue = 255 - greenValue;
            int yValue = 255 - blueValue;

            int CMYValue[3] = {cValue, mValue, yValue};

            int currentQuadrant = 0;

            if(( redValue + greenValue) > 255 ){
                if ((greenValue + blueValue) > 255){
                    if((redValue + greenValue + blueValue) > 510){
                        currentQuadrant = 1;
                    }
                    else{
                        currentQuadrant = 2;
                    }
                }
                else{
                    currentQuadrant = 3;
                }
            }

            else{
                if(!((greenValue + blueValue) > 255)){
                    if(!((redValue + greenValue + blueValue) > 255)){
                        currentQuadrant = 4;
                    }
                    else{
                        currentQuadrant = 5;
                    }
                }
                else{
                    currentQuadrant = 6;
                }
            }

           // std::cout<<"current quadrant: "<<currentQuadrant<<std::endl;
            get_minimum_vertex(CMYValue,currentQuadrant,currentLocation, outputChannel);

        }
    }
}


void CMYKColorSpace::get_minimum_vertex(int* CMYValue, int numQuad,int currentPixel, Image* outputChannel ){
    int Cvertex[3] = {0,255,0};
    int Mvertex[3] = {255,0,0};
    int Yvertex[3] = {0,0,255};
    int Rvertex[3] = {255,0,255};
    int Gvertex[3] = {0,255,255};
    int Bvertex[3] = {255,255,0};
    int Kvertex[3] = {0,0,0};
    int Wvertex[3] = {255,255,255};

    int minVertex[3];
    memset(minVertex , 0, 3 * sizeof(int));
    if(numQuad == 1){ //CMYW quadraple
        getMinimumDistance(CMYValue,Cvertex,Mvertex,Yvertex,Wvertex,minVertex);
    }
    if(numQuad == 2){ //mygc quadraple
        getMinimumDistance(CMYValue,Mvertex,Yvertex,Gvertex,Cvertex,minVertex);
    }
    if(numQuad == 3){ //rgmy quadraple
        getMinimumDistance(CMYValue,Rvertex,Gvertex,Mvertex,Yvertex,minVertex);
    }
    if(numQuad == 4){ //krgb quadraple
        getMinimumDistance(CMYValue,Kvertex,Rvertex,Gvertex,Bvertex,minVertex);
    }
    if(numQuad == 5){ //rgbm quadraple
        getMinimumDistance(CMYValue,Rvertex,Gvertex,Bvertex,Mvertex,minVertex);
    }
    if(numQuad == 6){ //cmgb quadraple
        getMinimumDistance(CMYValue,Cvertex,Mvertex,Gvertex,Bvertex,minVertex);
    }

   // std::cout<< "Vertexes :  "<<minVertex[0] <<" , "<<minVertex[1]<<" , "<<minVertex[2]<<std::endl;
    outputChannel->data[currentPixel + 0] = (unsigned char) minVertex[0];
    outputChannel->data[currentPixel + 1] = (unsigned char) minVertex[1];
    outputChannel->data[currentPixel + 2] = (unsigned char) minVertex[2];

}

void CMYKColorSpace::getMinimumDistance(int* CMYValue, int* first, int* sec, int* third,int* fourth, int* minVertex){
    double firstMin = 0;
    double secMin =0;
    double thridMin = 0;
    double fourthMin = 0;

    double finalMin = 0;
    for(int i = 0; i <3 ; i++){
        int fDiff = (CMYValue[i] - first[i]);
        double sqrdDiff = pow(fDiff , 2);
        sqrdDiff = sqrt(sqrdDiff);
        firstMin = firstMin + sqrdDiff;
       // std::cout<<"first: "<< firstMin<<std::endl;

        int sDiff = (CMYValue[i] - sec[i]);
        double ssqrdDiff = pow(sDiff , 2);
        ssqrdDiff = sqrt(ssqrdDiff);
        secMin = secMin + ssqrdDiff;
       // std::cout<<"sec: "<< secMin<<std::endl;


        int tDiff = (CMYValue[i] - third[i]);
        double tsqrdDiff = pow(tDiff , 2);
        tsqrdDiff = sqrt(tsqrdDiff);
        thridMin = thridMin + tsqrdDiff;
       // std::cout<<"third: "<< thridMin<<std::endl;

        int uDiff = (CMYValue[i] - fourth[i]);
        double usqrdDiff = pow(uDiff , 2);
        usqrdDiff = sqrt(usqrdDiff);
        fourthMin = fourthMin + usqrdDiff;
       // std::cout<<"four: "<< fourthMin<<std::endl;
    }


    if( (firstMin < secMin) && (firstMin < thridMin) && (firstMin < fourthMin) ){
        finalMin = firstMin;
        for(int i =0 ; i < 3; i++){
           // minVertex[i] = first[i];
            minVertex[i] = CMYValue[i] - first[i];
        }

    }
    else if( (secMin < firstMin) && (secMin < thridMin) && (secMin < fourthMin) ){
        finalMin = secMin;
        for(int i =0 ; i < 3; i++){
           // minVertex[i] = sec[i];
            minVertex[i] = CMYValue[i] - sec[i];
        }
    }
    else if( (thridMin < secMin) && (thridMin < firstMin) && (firstMin < fourthMin) ){
        finalMin = thridMin;
        for(int i =0 ; i < 3; i++){
          //  minVertex[i] = third[i];
            minVertex[i] = CMYValue[i] - third[i];
        }
    }
    else {
        for(int i =0 ; i < 3; i++){
          //  minVertex[i] = fourth[i];
            minVertex[i] = CMYValue[i] - fourth[i];
        }
    }
//std::cout<< " final Min : "<<finalMin <<std::endl;
  //  std::cout<< "Min Vertexes :  "<<minVertex[0] <<" , "<<minVertex[1]<<" , "<<minVertex[2]<<std::endl;

}

void CMYKColorSpace::combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight){
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

void CMYKColorSpace::all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel){
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