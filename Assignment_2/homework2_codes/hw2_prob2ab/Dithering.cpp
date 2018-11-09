//
// Created by Manasa Manohara on 2/9/18.
//
#include "Dithering.h"
#include <iostream>
#include<math.h>
#include <stdlib.h>
#include <random>

Dithering::Dithering(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

Dithering::~Dithering() {

}
void Dithering:: copy_image(Image* inputImage, Image* outputImage){
    for(int row =0; row < inputImage->height; row++ ){
        for(int col = 0; col < inputImage->height; col++ ){
            int currentLocation = ( row * inputImage->width) + col;
            outputImage->data[currentLocation] = inputImage->data[currentLocation];
        }
    }
}
void Dithering::fixed_thresholding(Image* inputImage, Image* outputImage){
    int threshold = 127;
    for(int row = 0; row < inputImage->height; row++){
        for(int col = 0; col < inputImage->width; col++){
            int currentPixel = ( row * inputImage->width ) + col;
            int currentLocation = currentPixel * inputImage->numChannels;
            int currentData = (int) inputImage->data[currentLocation];
            if(currentData < threshold ){
                outputImage->data[currentLocation] = 0;
            }
            else {
                outputImage->data[currentLocation] = 255;
            }
        }
    }
}

void Dithering::random_thresholding(Image* inputImage, Image* outputImage){
       /* std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<int> dist(0, 256);*/

    for(int row = 0; row < inputImage->height; row++){
        for(int col = 0; col < inputImage->width; col++){
            int currentPixel = ( row * inputImage->width ) + col;
            int currentLocation = currentPixel * inputImage->numChannels;
            int currentData = (int) inputImage->data[currentLocation];
            std::random_device rd;  //Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
            std::uniform_real_distribution<> dis(0, 256);
            int threshold  = (int) dis(gen);
            //int threshold = rand() %256;

            if(threshold < currentData + 1 ){
                outputImage->data[currentLocation] = 255;
            }
            else {
                outputImage->data[currentLocation] = 0;
            }
        }
    }
}
void Dithering:: get_bayer_matrix(const int* bayer_init, int* my_bayer_matrix, int matrixSize){

    for (int rowIndex = 0; rowIndex < matrixSize / 2; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize / 2; columnIndex++) {
            int basicMatrixLocation = (rowIndex * matrixSize / 2 ) + columnIndex;
            int currentValue = 4 * bayer_init[basicMatrixLocation] + 0;
            int bayerMatrixLocation = ( rowIndex * matrixSize ) + columnIndex;
            my_bayer_matrix[bayerMatrixLocation] = currentValue;
        }
    }

    for (int rowIndex = 0; rowIndex < matrixSize / 2; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize / 2; columnIndex++) {
            int basicMatrixLocation = (rowIndex * matrixSize / 2 ) + columnIndex;
            int currentValue = 4 * bayer_init[basicMatrixLocation] + 2;

            int bayerMatrixLocation = ( rowIndex * matrixSize ) + columnIndex+ (matrixSize/2);
            my_bayer_matrix[bayerMatrixLocation] = currentValue;

        }
    }

    for (int rowIndex = 0; rowIndex < matrixSize / 2; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize / 2; columnIndex++) {
            int basicMatrixLocation = (rowIndex * matrixSize / 2 ) + columnIndex;
            int currentValue = 4 * bayer_init[basicMatrixLocation] + 3;

            int bayerMatrixLocation = ( (rowIndex+ (matrixSize/2)) * matrixSize ) + columnIndex;
            my_bayer_matrix[bayerMatrixLocation] = currentValue;

        }
    }

    for (int rowIndex = 0; rowIndex < matrixSize / 2; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize / 2; columnIndex++) {

            int basicMatrixLocation = (rowIndex * matrixSize / 2 ) + columnIndex;
            int currentValue = 4 * bayer_init[basicMatrixLocation] + 1;

            int bayerMatrixLocation = ( (rowIndex+ (matrixSize/2)) * matrixSize ) + columnIndex + (matrixSize/2);
            my_bayer_matrix[bayerMatrixLocation] = currentValue;

        }
    }

}

void Dithering::compare_threshold(Image* inputImage, int* threshold_matrix, Image* outputImage, int size){
    for(int row = 0; row < inputImage->height; row++){
        for(int col = 0; col < inputImage->width; col++){
            int currentLocation = (row * inputImage->width) + col;
            int currentData = inputImage->data[currentLocation];

            int thresholdRow = row % size;
            int thresholdCol = col % size;

            int thresholdLocation = ( thresholdRow * size ) + thresholdCol;
            int thresholdValue = threshold_matrix[thresholdLocation];
            if(currentData > thresholdValue){
                outputImage->data[currentLocation] = 255;
            }
            else {
                outputImage->data[currentLocation] = 0;
            }


        }
    }
}
void Dithering::get_threshold_matrix(int* input, int size, int* output){
    int dimension = size * size;
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            int currentLocation = (row * size) + col;
            int currentData = input[currentLocation];
            double currentValue = ( (double) currentData + 0.5 ) / dimension;
            double intensityValue =  currentValue * 255;
            output[currentLocation] = (int) intensityValue;
        }
    }
}

void Dithering::order_dithering(Image* inputImage, Image* outputImage_4, Image* outputImage_8,Image* outputImage_2){
    int bayer_matrix_init[4] = {0,2,3,1};
    int matrixSize = 4;
    int matrixDimension = matrixSize*matrixSize;

    //get bayer matrices
    int my_bayer_matrix_4[matrixDimension];
    memset(my_bayer_matrix_4,0,matrixDimension * sizeof(int));
    get_bayer_matrix(bayer_matrix_init, my_bayer_matrix_4, matrixSize);

    int newmatrixSize = 8;
    int newmatrixDimension = newmatrixSize*newmatrixSize;

    int my_bayer_matrix_8[newmatrixDimension];
    memset(my_bayer_matrix_8,0,newmatrixDimension * sizeof(int));
    get_bayer_matrix(my_bayer_matrix_4, my_bayer_matrix_8, newmatrixSize);

    //get thresholding matrix

    int threshold_matrix_4[matrixDimension];
    memset(threshold_matrix_4, 0, matrixDimension * sizeof(int));

    int threshold_matrix_8[newmatrixDimension];
    memset(threshold_matrix_8, 0, newmatrixDimension * sizeof(int));

    int threshold_matrix_2[4];
    memset(threshold_matrix_2, 0, 4 * sizeof(int));


    get_threshold_matrix(my_bayer_matrix_4, 4, threshold_matrix_4 );
    get_threshold_matrix(my_bayer_matrix_8, 8, threshold_matrix_8 );
    get_threshold_matrix(bayer_matrix_init, 2, threshold_matrix_2 );


    ///compare values to threshold///////////////////////////////////

    compare_threshold(inputImage,threshold_matrix_4,outputImage_4, 4);
    compare_threshold(inputImage,threshold_matrix_8,outputImage_8, 8);
    compare_threshold(inputImage,threshold_matrix_2,outputImage_2, 2);





    //////get 2D arrays///////////////////////////////////////////////

    int** output_4_2DArrayAllocated = new int* [matrixSize];
    for(int i = 0; i < matrixSize; i++){
        output_4_2DArrayAllocated[i] = new int[matrixSize];
        memset(output_4_2DArrayAllocated[i], 0, matrixSize * sizeof(int));
    }
    int** output_4_2DArray = output_4_2DArrayAllocated;

    int** output_8_2DArrayAllocated = new int* [newmatrixSize];
    for(int i = 0; i < newmatrixSize; i++){
        output_8_2DArrayAllocated[i] = new int[newmatrixSize];
        memset(output_8_2DArrayAllocated[i], 0, newmatrixSize * sizeof(int));
    }
    int** output_8_2DArray = output_8_2DArrayAllocated;

    int** output_2_2DArrayAllocated = new int* [2];
    for(int i = 0; i < 2; i++){
        output_2_2DArrayAllocated[i] = new int[2];
        memset(output_2_2DArrayAllocated[i], 0, 2 * sizeof(int));
    }
    int** output_2_2DArray = output_2_2DArrayAllocated;


    get_2Darray(threshold_matrix_4, output_4_2DArray, matrixSize);
    get_2Darray(threshold_matrix_8, output_8_2DArray, newmatrixSize);
    get_2Darray(threshold_matrix_2, output_2_2DArray, 2);


}

void Dithering::get_2Darray(const int* inputArray, int** outputArray, int size){
    for(int row = 0; row < size; row++){
        for(int col = 0; col <size; col++){
            int currentPixelNumber =  (row * size) + col;
          //  int currentLocation = currentPixelNumber * inputArray->numChannels;
            outputArray[row][col] = inputArray[currentPixelNumber];
        }
    }
}

void Dithering::display_4_levels(Image* inputChannel, Image* outputChannel){

        int imgByteSize = inputChannel->width * inputChannel->height * inputChannel->numChannels;
    for(int row = 0; row < inputChannel->height; row++) {
        for (int col = 0; col < inputChannel->width; col++) {
            int currentPixelNumber =  (row * inputChannel->width) + col;
            int currtPixelLocation = currentPixelNumber * 1;
            int currentData = inputChannel->data[currtPixelLocation];

            if(currentData > 0 && currentData <=85){
                if(currentData < (int) 85/2){
                     outputChannel->data[currtPixelLocation] = 0;
                }
                else{
                    outputChannel->data[currtPixelLocation] = 85;
                }
            }
            else if(currentData > 85 && currentData <=170){
                if(currentData < (int) (170 +85)/2){
                    outputChannel->data[currtPixelLocation] = 85;
                }
                else{
                    outputChannel->data[currtPixelLocation] = 170;
                }
            }
            else if(currentData > 170 && currentData <=255){
                if(currentData < (int) (170 +255)/2){
                    outputChannel->data[currtPixelLocation] = 170;
                }
                else{
                    outputChannel->data[currtPixelLocation] = 255;
                }
            }
        }
    }



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dithering::get_neighbors( int row, int col, Image* reducedChannel,int* searchPixels, int*searchAddress, int neighborhood) {

    int searchStep = (neighborhood - 1) / 2;
    int currPixelLocation = (row * reducedChannel->width) + col;

    // Nothing to do with image channels because I am working with a single channel image here


    // I have to check for the neighborhood pixels to get the most frequent intensity and assign it to my output

    // Initialize mostFrequentIntensity to the current pixel intensity
   // unsigned char mostFrequentIntensity = reducedChannel->data[currPixelLocation];

    // Now go through the neighborhood and grab the most frequent intensity

    int searchRowStart = row - searchStep;
    int searchRowEnd = row + searchStep;

    int searchColStart = col - searchStep;
    int searchColEnd = col + searchStep;

    int searchPixelsCount = 0;
    int searchPixelsSize = neighborhood * neighborhood;


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
       //     std::cout<<" SearchPixelLocation "<<searchPixelLocation<< " searchPixelsCount "<<searchPixelsCount<<std::endl;
            searchPixels[searchPixelsCount] = (int) reducedChannel->data[searchPixelLocation];
            searchAddress[searchPixelsCount] = searchPixelLocation;
            searchPixelsCount++;
            //  std::cout<< " out searchPixelsCount++ "<< searchRow <<searchCol<<searchPixelsCount<<std::endl;
        }
    }

    searchPixelsCount = 0;
    for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
        for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
         //   std::cout<< searchCol << "," << searchRow << "(" << searchPixelsCount << ")" << searchPixels[searchPixelsCount] <<" out searchPixelsCount++ "<<searchAddress[searchPixelsCount]<< std::endl;
            searchPixelsCount++;
           // std::cout<< " out searchPixelsCount++ "<<searchAddress[searchPixelsCount]<<std::endl;
        }
    }

}

void Dithering::get_diffused_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels){
    double left_to_right_mask[9] = {0.0,0.0,0.0,0.0,0.0,(double)7/16, (double) 3/16, (double)5/16, (double) 1/16 };
    double right_to_left_mask[9] = { 0.0, 0.0, 0.0, (double) 7/16, 0.0, 0.0, (double) 1/16, (double) 5/16, (double)3/16};

    for(int i = 0; i< searchPixelsSize; i++){
       // std::cout<< "i "<<i<< " searchPixels  " <<searchPixels[i]<<std::endl;
        diffusedPixels[i] = searchPixels[i];
    }
    if(currentRow ==0) {
        // if the row is even
        for (int i = 5; i < searchPixelsSize; i++) {
            diffusedPixels[i] = searchPixels[i];
            if(left_to_right_mask[i] > 0 && searchPixels[i]>=0) {
                diffusedPixels[i] = floor( searchPixels[i] +left_to_right_mask[i]  * difference );
               // std::cout << " i " << i << "diffusedPixels[i][i] even" << diffusedPixels[i] << std::endl;
            }

        }
    }
    else{
        // if the row is odd
        for (int i = 3; i < searchPixelsSize; i++) {
            if(right_to_left_mask[i] > 0 && searchPixels[i]>=0){
                diffusedPixels[i] = floor (searchPixels[i] + right_to_left_mask[i]  * difference );
              //  std::cout << " i " << i << "diffusedPixels[i][i] odd" << diffusedPixels[i] << std::endl;
            }

        }
    }
    for(int i = 0; i < searchPixelsSize; i++ ){
      //std::cout<<" i "<< i << " diffusedPixels[i] " <<diffusedPixels[i]<<std::endl;
    }
    //diffusedPixels[4] = currentDataValue;

}

void Dithering::get_JJN_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels){
    double left_to_right_mask[25] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, (double)7/48,(double)5/48,(double)3/48,(double)5/48,(double)7/48,(double)5/48,(double)3/48,(double)1/48,(double)3/48,(double)5/48,(double)3/48,(double)1/48};
    double right_to_left_mask[25] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,(double)5/48,(double)7/48,0.0,0.0,0.0,(double)3/48,(double)5/48,(double)7/48,(double)5/48,(double)3/48,(double)1/48,(double)3/48,(double)5/48,(double)3/48,(double)1/48};

    for(int i = 0; i< searchPixelsSize; i++){
        // std::cout<< "i "<<i<< " searchPixels  " <<searchPixels[i]<<std::endl;
        diffusedPixels[i] = searchPixels[i];
    }
    if(currentRow ==0) {
        // if the row is even
        for (int i = 5; i < searchPixelsSize; i++) {
            diffusedPixels[i] = searchPixels[i];
            if(left_to_right_mask[i] > 0 && searchPixels[i]>=0) {
                diffusedPixels[i] = floor( searchPixels[i] +left_to_right_mask[i]  * difference );
                // std::cout << " i " << i << "diffusedPixels[i][i] even" << diffusedPixels[i] << std::endl;
            }

        }
    }
    else{
        // if the row is odd
        for (int i = 3; i < searchPixelsSize; i++) {
            if(right_to_left_mask[i] > 0 && searchPixels[i]>=0){
                diffusedPixels[i] = floor (searchPixels[i] + right_to_left_mask[i]  * difference );
                //  std::cout << " i " << i << "diffusedPixels[i][i] odd" << diffusedPixels[i] << std::endl;
            }

        }
    }
    for(int i = 0; i < searchPixelsSize; i++ ){
        //std::cout<<" i "<< i << " diffusedPixels[i] " <<diffusedPixels[i]<<std::endl;
    }
    //diffusedPixels[4] = currentDataValue;

}

void Dithering::get_Stucki_error(int currentDataValue, int difference, int currentRow, int searchPixelsSize,const int* searchPixels, double* diffusedPixels){
    double left_to_right_mask[25] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, (double)8.0/42.0,(double)4.0/42.0,(double)2.0/42.0, (double)4/42.0,(double)8/42.0,(double)4/42,(double)2/42, (double)1/42, (double)2/42,(double)4/42,(double)2/42,(double)1/42};
    double right_to_left_mask[25] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, (double)4.0/42.0,(double)8.0/42.0,0.0,0.0,0.0,(double)2.0/42.0, (double)4/42.0,(double)8/42.0,(double)4/42.0,(double)2/42, (double)1/42, (double)2/42,(double)4/42,(double)2/42,(double)1/42 };

    for(int i = 0; i< searchPixelsSize; i++){
        // std::cout<< "i "<<i<< " searchPixels  " <<searchPixels[i]<<std::endl;
        diffusedPixels[i] = searchPixels[i];
    }
    if(currentRow ==0) {
        // if the row is even
        for (int i = 5; i < searchPixelsSize; i++) {
            diffusedPixels[i] = searchPixels[i];
            if(left_to_right_mask[i] > 0 && searchPixels[i]>=0) {
                diffusedPixels[i] = round( searchPixels[i] +left_to_right_mask[i]  * difference );
                // std::cout << " i " << i << "diffusedPixels[i][i] even" << diffusedPixels[i] << std::endl;
            }

        }
    }
    else{
        // if the row is odd
        for (int i = 3; i < searchPixelsSize; i++) {
            if(right_to_left_mask[i] > 0 && searchPixels[i]>=0){
                diffusedPixels[i] = round (searchPixels[i] + right_to_left_mask[i]  * difference );
                //  std::cout << " i " << i << "diffusedPixels[i][i] odd" << diffusedPixels[i] << std::endl;
            }

        }
    }
    for(int i = 0; i < searchPixelsSize; i++ ){
        //std::cout<<" i "<< i << " diffusedPixels[i] " <<diffusedPixels[i]<<std::endl;
    }
    //diffusedPixels[4] = currentDataValue;

}

void Dithering::get_updated_image(Image* reducedChannel,int* searchPixels,double* diffusedPixels,int* searchAddress, int searchPixelsSize){
    for(int current = 0; current < searchPixelsSize; current++){
        if(searchPixels[current]> 0){
            int currentAddress = searchAddress[current];
            reducedChannel->data[currentAddress] = (unsigned char) diffusedPixels[current];
         // std::cout<< (int) reducedChannel->data[currentAddress]<< " reducedChannel->data[currentAddress] "<< currentAddress << " currentAddress"<<std::endl;

        }
    }
}

void Dithering::error_diffusion(bool isStucki,Image* reducedChannel, int neighborhood){


    int searchPixelsSize = neighborhood * neighborhood;
    int fromCol = 0;
    int toCol = 0;
    int inStep = 0;
    for (int row = 0; row < reducedChannel->height; row++) {
        int currentRow = row % 2;
     if(currentRow == 0){               //if even take the left to right traversal
         fromCol = 0;
         toCol = reducedChannel->width;
         inStep = 1;
     }
     else{
         fromCol = reducedChannel->width - 1; //if odd move right to left
         toCol = 0;
         inStep = -1;
     }

        for(int col = fromCol; col!= toCol; col+=inStep){
            int currentLocation = ( row * reducedChannel->width) + col;
            int currentDataValue = reducedChannel->data[currentLocation];
            int threshold = 127;
            int newCurrentValue = 0;
            if( currentDataValue > threshold){
                newCurrentValue = 255;
            }
            else{
                newCurrentValue = 0;
            }
            int difference =  currentDataValue - newCurrentValue;


            if( currentDataValue > threshold){
                reducedChannel->data[currentLocation] = 255;
            }
            else{
                reducedChannel->data[currentLocation] = 0;
            }

            int searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(int));
            int searchAddress[searchPixelsSize];
            memset(searchAddress, 0, searchPixelsSize * sizeof(int));
            double diffusedPixels[searchPixelsSize];
            memset(diffusedPixels, 0, searchPixelsSize * sizeof(double));
            if(neighborhood == 3 && isStucki == false){
                get_neighbors(row, col, reducedChannel, searchPixels, searchAddress, neighborhood);
                get_diffused_error(neighborhood, difference, currentRow, searchPixelsSize, searchPixels, diffusedPixels);
                get_updated_image(reducedChannel,searchPixels,diffusedPixels,searchAddress,searchPixelsSize);

            }

            if(neighborhood == 5 && isStucki == false){
                get_neighbors(row, col, reducedChannel, searchPixels, searchAddress, neighborhood);
                get_JJN_error(neighborhood, difference, currentRow, searchPixelsSize, searchPixels, diffusedPixels);
                get_updated_image(reducedChannel,searchPixels,diffusedPixels,searchAddress,searchPixelsSize);
            }

            if(neighborhood == 5 && isStucki == true){
                get_neighbors(row, col, reducedChannel, searchPixels, searchAddress, neighborhood);
                get_Stucki_error(neighborhood, difference, currentRow, searchPixelsSize, searchPixels, diffusedPixels);
                get_updated_image(reducedChannel,searchPixels,diffusedPixels,searchAddress,searchPixelsSize);
            }
         }


    }
 /*  int i = 0;
    for (int row = 0; row < reducedChannel->height; row++) {
        for (int col = 0; col < reducedChannel->width; col++) {
            int currPixelLocation = (row * reducedChannel->width) + col;
            std::cout<< (int) reducedChannel->data[currPixelLocation]<< " reducedChannel->data[currentAddress] hereNOTTTT "<< currPixelLocation << " currentAddress"<< " i "<< i <<std::endl;
            i++;

        }
    }*/
 /*   FILE* file = nullptr;
    file=fopen(fileName.c_str(),"wb");
    int imgSize = reducedChannel->width * reducedChannel->height * reducedChannel->numChannels;
    if (file == nullptr) {
        std::cout << "Cannot open file: " << fileName << std::endl;
        exit(1);
    }
    fwrite(reducedChannel->data, sizeof(unsigned char), imgSize, file);
    fclose(file);*/
}
