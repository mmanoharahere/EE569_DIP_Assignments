//
// Created by Manasa Manohara on 2/9/18.
//
//
// Created by Manasa Manohara on 1/19/18.
//
#include "SingleChannel.h"
#include <iostream>
#include<cmath>


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

void SingleChannel::get_2Darray(Image* inputChannel, unsigned char** outputArray){
    for(int row = 0; row < inputChannel->height; row++){
        for(int col = 0; col < inputChannel->width; col++){
            int currentPixelNumber =  (row * inputChannel->width) + col;
            int currentLocation = currentPixelNumber * inputChannel->numChannels;
            outputArray[row][col] = inputChannel->data[currentLocation];
        }
    }
}

void SingleChannel::convertToCircle(unsigned char** inputArray, Image* outputChannel, Image* reversedChannel, int imgWidth, int imgHeight){

    //get your indexes to be between -1 and +1
    float middleRow = (imgHeight / 2.0 ) - 1;
    float middleCol = (imgWidth / 2.0 ) - 1;

    //Allocate memory for the translated 2D array

    unsigned char** output2DArrayAllocated = new unsigned char* [imgHeight];
    for(int i = 0; i < imgHeight; i++){
        output2DArrayAllocated[i] = new unsigned char[imgWidth];
        memset(output2DArrayAllocated[i], 0, imgWidth * sizeof(unsigned char));
    }
    unsigned char** output2DArray = output2DArrayAllocated;

    unsigned char** reverse2DArrayAllocated = new unsigned char* [imgHeight];
    for(int i = 0; i < imgHeight; i++){
        reverse2DArrayAllocated[i] = new unsigned char[imgWidth];
        memset(reverse2DArrayAllocated[i], 0, imgWidth * sizeof(unsigned char));
    }
    unsigned char** reverse2DArray = reverse2DArrayAllocated;

    for(int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {

            unsigned char currentData = inputArray[row][col];


            float currentRow = (row - middleRow) / middleRow;
            float currentCol = (col - middleCol) / middleCol;

            float rowSquare = currentRow * currentRow;
            float colSquare = currentCol * currentCol;

            float translatedRowDifference = (1.0 - (colSquare /2.0));
            float translatedColDifference =  (1.0 - (rowSquare/2.0));

            float translatedRow = currentRow * (sqrt(translatedRowDifference));
            float translatedCol = currentCol * (sqrt(translatedColDifference));

            float circleIntRow = (translatedRow * middleRow ) + middleRow;
            float circleIntCol = (translatedCol * middleCol ) + middleCol;

            int circleNewRow = (int)circleIntRow;
            int circleNewCol = (int) circleIntCol;

            output2DArray[circleNewRow][circleNewCol] = currentData;


            //

          /*  g=(i-((imageHeight/2)-1))/(float)(((imageHeight/2)-1));

            h=(j-((imageWidth/2)-1))/(float)(((imageWidth/2)-1));

            u=g*sqrt(1-(pow(h,2)/2));

            v=h*sqrt(1-(pow(g,2)/2));*/


            //convert it back
            float circleMidRow =  middleRow;
            float circleMidCol =  middleCol;

           /* double circleRow = (double)(circleNewRow - circleMidRow) / circleMidRow;
            double circleCol = (double) (circleNewCol - circleMidCol) / circleMidCol;*/

            float circleRow = translatedRow;
            float circleCol = translatedCol;

            float squareRow = 0.0;
            float squareCol = 0.0;
            squareRow =1.00/2.00*((sqrt((2+(pow(circleRow,2))-(pow(circleCol,2))+(2*(sqrt(2)*circleRow)))))-(sqrt((2+(pow(circleRow,2))-(pow(circleCol,2))-(2*(sqrt(2)*circleRow))))));

            squareCol=1.00/2.00*((sqrt((2-(pow(circleRow,2))+(pow(circleCol,2))+(2*(sqrt(2)*circleCol)))))-(sqrt((2-(pow(circleRow,2))+(pow(circleCol,2))-(2*(sqrt(2)*circleCol))))));


            float finalRow  = round((squareRow * circleMidRow)  + circleMidRow) ;
            float finalCol = round((squareCol * circleMidCol)  + circleMidCol);

            auto squareRowIndex = (int) (finalRow);
            auto squareColIndex = (int) (finalCol);

           // std::cout<<row<< " , "<< col<< "    "<< squareRowIndex<< " , "<< squareColIndex<<std::endl;
           // std::cout<<squareRow<<" , "<<squareCol<< "    "<< squareRowIndex<< " , "<<squareColIndex<<std::endl;
            if(row==squareRowIndex && col==squareColIndex){
                if(row<255 &&col<255){
                    //std::cout<<currentRow<< " , "<< currentCol<< "    "<< squareRowIndex<< " , "<< squareColIndex<<std::endl;
                }

            }
            unsigned char currentCircleData = output2DArray[circleNewRow][circleNewCol];
            reverse2DArray[squareRowIndex][squareColIndex] = currentCircleData;

        }
    }

    for(int circleCurrentRow = 0; circleCurrentRow < imgHeight; circleCurrentRow++){
        for(int circleCurrentCol = 0; circleCurrentCol < imgWidth; circleCurrentCol++){
            int currentPixelNumber =  (circleCurrentRow * imgWidth) + circleCurrentCol;
            int currentLocation = currentPixelNumber * 1;
            outputChannel->data[currentLocation] = output2DArray[circleCurrentRow][circleCurrentCol];
            reversedChannel->data[currentLocation] = reverse2DArray[circleCurrentRow][circleCurrentCol];
        }
    }
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


