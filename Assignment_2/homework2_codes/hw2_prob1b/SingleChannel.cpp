//
// Created by Manasa Manohara on 2/10/18.
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


void SingleChannel::embed_image(Image* inputChannel, Image*outputChannel){
    int currentPixel = 0;
    int beginCol = 795 ;
    int tillCol = beginCol + inputChannel->width ;
    int beginRow = 300;
    int tillRow = beginRow + inputChannel->height ;

    for(int embedRow = beginRow; embedRow < tillRow; embedRow++){
        for(int embedCol = beginCol; embedCol < tillCol; embedCol++){
                int currentEmbedpixel = (embedRow * outputChannel->width) + embedCol;
                int currentEmbedLocation = currentEmbedpixel * outputChannel->numChannels;
                outputChannel->data[currentEmbedLocation + 0] = inputChannel->data[currentPixel + 0];
                outputChannel->data[currentEmbedLocation + 1] = inputChannel->data[currentPixel + 1];
                outputChannel->data[currentEmbedLocation + 2] = inputChannel->data[currentPixel + 2];

                currentPixel = currentPixel+3;
        }
    }


}

void SingleChannel::homographic_trans(float* H_array,Image* originalImg, Image* outputChannel ,  int imgWidth, int imgHeight,int embedWidth, int embedHeight){
    //Homographic matrix
    //host img - left.png 480x640
    //text img - middle.png


    int H_matrix_height = 3;
    int H_matrix_width = 3;

    //2D H Matrix
    float** H_2DArrayAllocated = new float* [H_matrix_height];
    for(int i = 0; i < H_matrix_height; i++){
        H_2DArrayAllocated[i] = new float[H_matrix_width];
        memset(H_2DArrayAllocated[i], 0, H_matrix_width * sizeof(float));
    }
    float** H_2DArray = H_2DArrayAllocated;



    for(int i = 0; i < H_matrix_height; i++){
        for(int j = 0; j< H_matrix_width; j++){
            int currentPixelNumber =  (i * H_matrix_width) + j;
            H_2DArray[i][j] = H_array[currentPixelNumber];

        }
    }

    long double weight = 0;

    for(int row = 0; row < embedHeight + 1; row++) {
        for (int col = 0; col < embedWidth + 1 ; col++) {
            int outPixelNumber = (col + (row * embedWidth));
            int outPixelLocation = outPixelNumber * 3;

            int vectorHeight = 3;
            int datavec[3] = {row,col,1};

            if (H_matrix_width == vectorHeight) {
                long double outputArray[3];
                memset(outputArray, 0, 3 * sizeof(long double));

                 weight = 1/((H_2DArray[2][0] * row)+(H_2DArray[2][1] * col) + H_2DArray[2][2]);
               // std::cout<<"weight "<<weight<<std::endl;

                for (int mulrow = 0; mulrow < H_matrix_height; mulrow++) {
                    for (int mulcol = 0; mulcol < vectorHeight; mulcol++) {
                        outputArray[mulrow] = outputArray[mulrow] + H_2DArray[mulrow][mulcol] * datavec[mulcol];
                    }

                }
             //   std::cout<<outputArray[0]<<" , "<<  outputArray[1]<< "  , "<< outputArray[2]<<std::endl;
               float new_x = (float)outputArray[0] * (float)weight;
                float new_y = (float)outputArray[1] * (float)weight;
             //  std::cout<< new_y << " , "<<new_x<<std::endl;
             //   std::cout<<row << " row ,col "<<col<<std::endl;
                bilinear_interpolate(originalImg, outputChannel,new_x,new_y,imgHeight,imgWidth, outPixelLocation);


            }
        }
    }

}


void SingleChannel::bilinear_interpolate(Image* originalImg,Image* outputChannel, float new_x,float new_y, int imgHeight, int imgWidth, int outPixelLocation){
    int floorRow = floor(new_x);
    int floorColumn = floor(new_y);

    float deltaRow = new_x - floorRow;

    float deltaColumn = new_y - floorColumn;


    if((floorRow > 0 ) && (floorRow < imgHeight) && (floorColumn > 0) && (floorColumn < imgWidth)) {


        int topLeftPixelNumber = ((floorRow * imgWidth) + floorColumn);
        int topLeftPixelLocation = (int)topLeftPixelNumber * 3;

        int bottomLeftPixelNumber = (int)((floorRow * imgWidth) + floorColumn + imgWidth);
        int bottomLeftPixelLocation = (int)bottomLeftPixelNumber * 3;

        int topRightPixelNumber = (int)((floorRow * imgWidth) + floorColumn + 1);
        int topRightPixelLocation = (int)topRightPixelNumber * 3;

        int bottomRightPixelNumber =(int)((floorRow * imgWidth) + floorColumn + imgWidth + 1);
        int bottomRightPixelLocation = (int)bottomRightPixelNumber * 3;


        unsigned char currentValueRed =   (unsigned char) (( originalImg->data[topLeftPixelLocation + 0] * (1 - deltaRow) * (1 - deltaColumn) )
                                                           +( originalImg->data[bottomLeftPixelLocation + 0] * (deltaRow) * (1 - deltaColumn) )
                                                           +( originalImg->data [topRightPixelLocation + 0] * (1 - deltaRow) * (deltaColumn) )
                                                           + ( originalImg->data[bottomRightPixelLocation + 0] * (deltaRow) * (deltaColumn)));

        unsigned char currentValueGreen = (unsigned char)  ( ( originalImg->data[topLeftPixelLocation + 1] * (1 - deltaRow) * (1 - deltaColumn) )
                                                             +( originalImg->data[bottomLeftPixelLocation + 1] * (deltaRow) * (1 - deltaColumn) )
                                                             + ( originalImg->data [topRightPixelLocation + 1] * (1 - deltaRow) * (deltaColumn) )
                                                             + ( originalImg->data[bottomRightPixelLocation + 1] * (deltaRow) * (deltaColumn)));

        unsigned char currentValueBlue = (unsigned char) ( ( originalImg->data[topLeftPixelLocation + 2] * (1 - deltaRow) * (1 - deltaColumn) )
                                                           +( originalImg->data[bottomLeftPixelLocation + 2] * (deltaRow) * (1 - deltaColumn) )
                                                           +( originalImg->data [topRightPixelLocation + 2] * (1 - deltaRow) * (deltaColumn) )
                                                           +( originalImg->data[bottomRightPixelLocation + 2] * (deltaRow) * (deltaColumn)));
        outputChannel->data[outPixelLocation + 0] = currentValueRed;
        outputChannel->data[outPixelLocation + 1] = currentValueGreen;
        outputChannel->data[outPixelLocation + 2] = currentValueBlue;

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

