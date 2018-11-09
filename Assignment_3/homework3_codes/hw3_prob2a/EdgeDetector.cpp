//
// Created by Manasa Manohara on 3/14/18.
//

//
// Created by Manasa Manohara on 3/13/18.
//

//
// Created by Manasa Manohara on 2/9/18.
//
//
// Created by Manasa Manohara on 1/19/18.
//
#include "opencv2/highgui.hpp"
#include "EdgeDetector.h"
#include <iostream>
#include<cmath>
using namespace cv;

EdgeDetector::EdgeDetector(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

EdgeDetector::~EdgeDetector(){
}






void EdgeDetector:: all_channel(Image* redChannel , Image* greenChannel, Image* blueChannel){
    for (int row = 0; row < this->inImg->height; row++) {
        for (int col = 0; col < this->inImg->width; col++) {
            int currentPixelNumber =  (row * this->inImg->width) + col;
            int currentLocation = currentPixelNumber * this->inImg->numChannels;
            int EdgeDetectorCurrentLocation = currentPixelNumber * 1;

            redChannel->data[EdgeDetectorCurrentLocation] = this->inImg->data[currentLocation + 0];
            greenChannel->data[EdgeDetectorCurrentLocation] = this->inImg->data[currentLocation + 1];
            blueChannel->data[EdgeDetectorCurrentLocation] = this->inImg->data[currentLocation + 2];
        }
    }

}

void EdgeDetector::get_2Darray(Image* inputChannel, unsigned char** outputArray){
    for(int row = 0; row < inputChannel->height; row++){
        for(int col = 0; col < inputChannel->width; col++){
            int currentPixelNumber =  (row * inputChannel->width) + col;
            int currentLocation = currentPixelNumber * inputChannel->numChannels;
            outputArray[row][col] = inputChannel->data[currentLocation];
        }
    }
}


void EdgeDetector::combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight){
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


void EdgeDetector::texture_analysis(Image* inputImg, Image* sobeloutputImg, Image* xgradImg, Image* ygradImg){
    int matrixSize = 3;
    int matrixDimension = matrixSize * matrixSize;


    int neighborhood = 3;
    //  double E5[5] = {-1, -2, 0, 2, 1};
    //  double S5[5] = {1,4,6,4,1};

    //pseudocode
    //get the nearest neighbor
    //perform convolution
    //find the added sum of individual gradients
    //take mean square and apply to a new image

    //get the nearest neighbors

    //set minimum and maximum x and y gradient values
    //get the returned x and y gradient values
    //compare with returned x and y values
    //set the updated x and y values
    //for X
    double maxXValue = 0;
    double minXValue = 0;
    //for Y
    double maxYValue = 0;
    double minYValue = 0;
    //for Magnitude
    double maxMagValue = 0;
    double minMagValue = 0;

    //returning values
   /* double returnedXValue = 0;
    double returnedYValue = 0;
    double returnedMagValue = 0;*/

    double x_value = 0;
    double y_value = 0;
    double magValue = 0;





    for (int row = 0; row < inputImg->height; row++) {
        for (int col = 0; col < inputImg->width; col++) {

            int currentLocation = (row*inputImg->width) + col;
            int searchPixels[matrixDimension];
            memset(searchPixels,0,matrixDimension * sizeof(int));
            nn_assign(inputImg, searchPixels, inputImg->width, inputImg->height, neighborhood, row, col);
           // std::cout<<row<<" " <<col<<std::endl;
            x_value = x_gradient(searchPixels, currentLocation);
            y_value = y_gradient(searchPixels,currentLocation);
            magValue = sobelOperator(searchPixels,  currentLocation);
          //  std::cout<< " Returned values : "<<x_value<< " , "<<y_value<<" , "<<magValue<<std::endl;
             if(row == 0 & col == 0){
                 minXValue = x_value;
                 maxXValue = x_value;

                 minYValue = y_value;
                 maxYValue = y_value;

                 minMagValue = magValue;
                 maxMagValue = magValue;
                }
                else{
                 if(x_value < minXValue){
                     minXValue = x_value;
                 }
                 if(x_value > maxXValue){
                     maxXValue = x_value;
                 }
                 if(y_value < minYValue){
                     minYValue = y_value;
                 }
                 if(y_value > maxYValue){
                     maxYValue = y_value;
                 }
                 if(magValue < minMagValue){
                     minMagValue = magValue;
                 }
                 if(magValue > maxMagValue){
                     maxMagValue = magValue;
                 }


             }

        }
    }

    //now update images

    for (int row = 0; row < inputImg->height; row++) {
        for (int col = 0; col < inputImg->width; col++) {
            int currentLocation = (row*inputImg->width) + col;
            int searchPixels[matrixDimension];
            memset(searchPixels,0,matrixDimension * sizeof(int));
            nn_assign(inputImg, searchPixels, inputImg->width, inputImg->height, neighborhood, row, col);
            sobelOutput(searchPixels, currentLocation, maxXValue,  minXValue, maxYValue, minYValue,  maxMagValue, minMagValue, sobeloutputImg, xgradImg,  ygradImg);

        }
    }


}
void EdgeDetector::sobelOutput(const int* searchPixels,  int currentLocation, double maxX, double minX, double maxY, double minY, double maxMag, double minMag, Image* outputImg, Image* xgradImg, Image* ygradImg){

    double grad_x[9] = {-1,0,1,-2,0,2,-1,0,1};
    double grad_y[9] = {1,2,1,0,0,0,-1,-2,-1};

    double maxValue = 0;
    double xgradValue = 0;
    double ygradValue = 0;
    int searchPixelsSize = 9;
    double x_sum = 0;
    double y_sum = 0;
    //multiply pixels - convolution

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            //get x gradient
            double x_value = searchPixels[i] * grad_x[i];
            x_sum = x_sum + (x_value );

            //get y gradient
            double y_value = searchPixels[i] * grad_y[i];
            y_sum = y_sum + (y_value );


            //std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }


    double sqrx = pow(x_sum,2);
    double sqry = pow(y_sum,2);

    double addSum = sqrx + sqry;
    double sqrtValue = sqrt(addSum);

    double newValue = sqrtValue;

    //std::cout<<"Max Value : "<<maxValue<<std::endl;


    // x gradient value
    xgradValue = ( x_sum - minX )/ (maxX - minX);
    xgradValue = xgradValue * 255;

    //y gradient value

    //std::cout<<newValue<<std::endl;
    ygradValue = ( y_sum - minY )/ (maxY - minY);
    ygradValue = ygradValue * 255;


    //new value
    newValue = ( newValue - minMag )/ (maxMag - minMag);
    newValue = newValue * 255;
    //std::cout<<newValue<<std::endl;



    outputImg->data[currentLocation] = (unsigned char) newValue;
    xgradImg->data[currentLocation] = (unsigned char) xgradValue;
    ygradImg->data[currentLocation] = (unsigned char) ygradValue;

}
double EdgeDetector::sobelOperator(const int* searchPixels, int currentLocation){

    double grad_x[9] = {-1,0,1,-2,0,2,-1,0,1};
    double grad_y[9] = {1,2,1,0,0,0,-1,-2,-1};
    int searchPixelsSize = 9;
    double x_sum = 0;
    double y_sum = 0;
    //multiply pixels - convolution

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            //get x gradient
            double x_value = searchPixels[i] * grad_x[i];
            x_sum = x_sum + (x_value );

            //get y gradient
            double y_value = searchPixels[i] * grad_y[i];
            y_sum = y_sum + (y_value );


            //std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }


    double sqrx = pow(x_sum,2);
    double sqry = pow(y_sum,2);

    double addSum = sqrx + sqry;
    double sqrtValue = sqrt(addSum);

    double newValue = sqrtValue;



    //newValue = ( newValue - 0 )/ 923;
   // newValue = newValue * 255;
   // outputImg->data[currentLocation] = (unsigned char) newValue;
    return newValue;

}


double EdgeDetector::x_gradient(const int* searchPixels,  int currentLocation){

    double grad_x[9] = {-1,0,1,-2,0,2,-1,0,1};
   // double grad_y[9] = {1,2,1,0,0,0,-1,-2,-1};
    int searchPixelsSize = 9;
    double x_sum = 0;
   // double y_sum = 0;
    //multiply pixels - convolution

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            //get x gradient
            double x_value = searchPixels[i] * grad_x[i];
            x_sum = x_sum + (x_value );

            //std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }


   return x_sum;

}

double EdgeDetector::y_gradient(const int* searchPixels,  int currentLocation){

    //double grad_x[9] = {-1,0,1,-2,0,2,-1,0,1};
     double grad_y[9] = {1,2,1,0,0,0,-1,-2,-1};
    int searchPixelsSize = 9;
   // double x_sum = 0;
     double y_sum = 0;
    //multiply pixels - convolution

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            //get x gradient
            double y_value = searchPixels[i] * grad_y[i];
            y_sum = y_sum + (y_value );

            //std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }


    return y_sum;

}



void EdgeDetector::removeDC(Image* inputImg, double* reducedDCImg){
    int imgSize = inputImg->width * inputImg->height;
    int sumIntensities = 0;
    for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
            int currentPixel = (row * inputImg->width) + col;
            int currLocation = currentPixel * 1;
            sumIntensities = sumIntensities + inputImg->data[currLocation];
        }
    }

    int meanIntensity = sumIntensities / imgSize;

    for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
            int currentPixel = (row * inputImg->width) + col;
            int currLocation = currentPixel * 1;
            reducedDCImg[currLocation] = (inputImg->data[currLocation] - meanIntensity);
        }
    }
}

void EdgeDetector::nn_assign(Image* reducedChannel, int* searchPixels, int imgWidth, int imgHeight, int neighborhood, int row, int col){
    int searchStep = (neighborhood - 1)/2;

            int currPixelLocation = (row * imgWidth) + col;

            int searchRowStart = row - searchStep;
            int searchRowEnd = row + searchStep;

            int searchColStart = col - searchStep;
            int searchColEnd = col + searchStep;

            int searchPixelsCount = 0;
            int searchPixelsSize = neighborhood * neighborhood;



    for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
        for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

            int newCol = 0;
            int newRow = 0;
            if (searchCol < 0 || searchCol >= imgWidth) {
                //searchPixels[searchPixelsCount] = -800;
                //searchPixelsCount++;

                if(searchCol < 0){
                    newCol = 0;
                }
                if(searchCol >= imgWidth){
                    newCol = imgWidth -  1;
                }
                // searchPixelsCount++;
                //continue;
            }
            else{
                newCol = searchCol;
            }

            if (searchRow < 0 || searchRow >= imgHeight) {
                // searchPixels[searchPixelsCount] = -800;
                //searchPixelsCount++;

                if(searchRow < 0){
                    newRow = 0;
                }
                if(searchRow >= imgHeight){
                    newRow = imgHeight - 1;
                }
                // searchPixelsCount++;
                // continue;
            }
            else{
                newRow = searchRow;
            }


            int searchPixelLocation = (newRow * imgWidth) + newCol;
            searchPixels[searchPixelsCount] = reducedChannel->data[searchPixelLocation];
            //std::cout<< newRow<<", "<<newCol<< " : "<<searchPixels[searchPixelsCount]<<std::endl;
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

//take top 10% as threshold. Anything lesser than threshold make it 0 else 255


void EdgeDetector::log_analysis(Image* inputImg, Image* outputImage,const std::string &fileName){
    int matrixSize = 9;
    int matrixDimension = matrixSize * matrixSize;

    int imgSize = inputImg->height * inputImg->width;

    int neighborhood = matrixSize ;
    //  double E5[5] = {-1, -2, 0, 2, 1};
    //  double S5[5] = {1,4,6,4,1};

    //pseudocode
    //get the nearest neighbor
    //perform convolution

    int normArray[imgSize];
    //get the nearest neighbors
    int magnitude[imgSize];
    memset(magnitude,0,imgSize * sizeof(int));

    for (int row = 0; row < inputImg->height; row++) {
        for (int col = 0; col < inputImg->width; col++) {

            int currentLocation = (row*inputImg->width) + col;
            int searchPixels[matrixDimension];
            memset(searchPixels,0,matrixDimension * sizeof(int));
            nn_assign(inputImg, searchPixels, inputImg->width, inputImg->height, neighborhood, row, col);
            //std::cout<<row<<" " <<col<<std::endl;

            double currValue = logOperator(searchPixels, magnitude, currentLocation);
            magnitude[currentLocation] = currValue;
            //std::cout<<" Magnitude : " <<currentLocation<<" : " <<magnitude[currentLocation]<<std::endl;


        }
    }
    memset(normArray,0,imgSize * sizeof(int));
    normalize_values(magnitude, normArray,inputImg->width, inputImg->height);
    int frequency[256];
    memset(frequency,0,256 * sizeof(int));
    calc_freq(normArray, inputImg->height, inputImg->width,frequency);
    print_histogram(fileName,frequency);

    //cdf values
    int cdf[256];
    memset(cdf,0,256 * sizeof(int));
    cdf[0]=frequency[0];
    for(int i=0;i<256;i++)

    {
        if(i>0)

        {
            cdf[i]=cdf[i-1]+frequency[i];
            //std::cout<< " i: "<<i<<"CDF : "<<cdf[i]<<std::endl;

        }

    }


    Image* thresholdLog = new Image(inputImg->width, inputImg->height, 1);
    thresholdLog->allocate();
    thresholdLog->name=("LOg grayscale");

    int thresholdArray[imgSize];
    memset(thresholdArray,0,imgSize * sizeof(int));

    get_thresholds(normArray, thresholdLog,  thresholdArray, inputImg->width, inputImg->height);

    int mulNeigh = 3;
    int searchPixelsSize = mulNeigh * mulNeigh;
    
    int searchPixels[searchPixelsSize];
    memset(searchPixels, 0, searchPixelsSize * sizeof(int));

    nn_assign_multiply(thresholdArray,searchPixels, inputImg->width, inputImg->height, mulNeigh,outputImage);
    //zero_crossing(searchPixels, outputImg, currLocation);


//display_help(*thresholdLog);

}



double EdgeDetector::logOperator(const int* searchPixels, int* outputImg, int currentLocation){

    //change to 9x9; 122 156 thresholds
    //center pixel needs to be 0 - check 8 conectivity neighborhood for 1 or -1
    //if center pixel value not zero - output = 255
 //   double log_mask[9] = {0,-1,0,-1,4,-1,0,-1,0};

    double log_mask[81] = {0,1,1,2,2,2,1,1,0,1,2,4,5,5,5,4,2,1,1,4,5,3,0,3,5,4,1,2,5,3,-12,-24,-12,3,5,2,2,5,0,-24,-40,-24,0,5,2,2,5,3,-12,-24,-12,3,5,2,1,4,5,3,0,3,5,4,1,1,2,4,5,5,5,4,2,1,0,1,1,2,2,2,1,1,0};

   // double grad_y[9] = {1,2,1,0,0,0,-1,-2,-1};

    int searchSize = 9;
    int searchPixelsSize = searchSize * searchSize ;
    double sum = 0;

    //multiply pixels - convolution

    for (int i = 0; i < searchPixelsSize; i++) {
        int currPixelValue = searchPixels[i];
       // if (currPixelValue != -800) {}
            //get x gradient
            double x_value = searchPixels[i] * log_mask[i];
            sum = sum + (x_value );

           // std::cout<<"Sum array : "<<sum<< " , "<< searchPixels[i] << " , "<< log_mask[i]<<std::endl;

    }


return sum;


}

//normalize the values
void EdgeDetector::normalize_values(int* magnitude, int* normArray,int imgWidth, int imgHeight){
    int minValue = magnitude[0];
    int maxValue = magnitude[0];

    for(int row = 0; row < imgHeight; row++){
        for(int col = 0; col < imgWidth; col++){
            int currLocation = (row * imgWidth) + col;

            if(maxValue < magnitude[currLocation]){
                maxValue = magnitude[currLocation];
            }

            if(minValue > magnitude[currLocation]){
                minValue = magnitude[currLocation];
            }
        }
    }
//std::cout<<"Max : "<<maxValue<<" Min : "<<minValue<<std::endl;
    for(int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currLocation = (row * imgWidth) + col;
            normArray[currLocation] = (int)(255*((double)(magnitude[currLocation]-minValue)/(double)(maxValue-minValue)));
            //std::cout<<"Normalized : "<<normArray[currLocation]<<" , "<<currLocation<<std::endl;
        }
    }
}

void EdgeDetector::calc_freq(int* normArray, int imgHeight, int imgWidth, int* frequency) {
    for (int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currentPixelNumber = (col + (row * imgWidth));
            int currentPixelLocation = currentPixelNumber * 1;

            int currentPixelValue = normArray[currentPixelLocation];

            // Update the frequency of the currentPixelValue
            frequency[currentPixelValue]++;

        }
    }


}

void EdgeDetector::get_thresholds(int* normalizedArray, Image* thresholdImg, int* thresholdInt, int imageWidth, int imageHeight){

    for(int row=0;row<imageHeight;row++)
    {
        for(int col=0;col<imageWidth;col++)
        {
            int currlocation = (row * imageWidth) + col;

            if(normalizedArray[currlocation]<122)
            {
                thresholdImg->data[currlocation]=64;
                thresholdInt[currlocation]=-1;

            }
            if(normalizedArray[currlocation]>=122 && normalizedArray[currlocation]<=156)
            {
                thresholdImg->data[currlocation]=128;
                thresholdInt[currlocation]=0;
            }
            if(normalizedArray[currlocation]>156)
            {
                thresholdImg->data[currlocation]=192;
                thresholdInt[currlocation]=1;
            }

        }

    }
}


void EdgeDetector::nn_assign_multiply(int* reducedChannel,int* searchPixels, int imgWidth, int imgHeight, int neighborhood, Image* outputImage){
    int searchStep = (neighborhood - 1)/2;
    std::cout<<imgWidth<<" ; "<<imgHeight<<std::endl;
    for (int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currPixelLocation = (row * imgWidth) + col;

            int searchRowStart = row - searchStep;
            int searchRowEnd = row + searchStep;

            int searchColStart = col - searchStep;
            int searchColEnd = col + searchStep;

            int searchPixelsCount = 0;
            int searchPixelsSize = neighborhood * neighborhood;

            

            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    int newCol = 0;
                    int newRow = 0;
                    if (searchCol < 0 || searchCol >= imgWidth) {
                        //searchPixels[searchPixelsCount] = -800;
                        //searchPixelsCount++;

                        if(searchCol < 0){
                            newCol = 0;
                        }
                        if(searchCol >= imgWidth){
                            newCol = imgWidth -  1;
                        }
                        // searchPixelsCount++;
                        //continue;
                    }
                    else{
                        newCol = searchCol;
                    }

                    if (searchRow < 0 || searchRow >= imgHeight) {
                        // searchPixels[searchPixelsCount] = -800;
                        //searchPixelsCount++;

                        if(searchRow < 0){
                            newRow = 0;
                        }
                        if(searchRow >= imgHeight){
                            newRow = imgHeight - 1;
                        }
                        // searchPixelsCount++;
                        // continue;
                    }
                    else{
                        newRow = searchRow;
                    }


                    int searchPixelLocation = (newRow * imgWidth) + newCol;
                    searchPixels[searchPixelsCount] = reducedChannel[searchPixelLocation];
                    // std::cout<< newRow<<", "<<newCol<< " : "<<searchPixels[searchPixelsCount]<<std::endl;
                    searchPixelsCount++;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    searchPixelsCount++;
                }
            }
            zero_crossing(searchPixels, outputImage, currPixelLocation);
        }
    }




    //  I AM AWSEOME PAWSOME, you better believe it!! yeah!
}

void EdgeDetector::zero_crossing(int* searchPixels, Image* outputImg, int currLocation){
    bool ifEdge = false;
    int searchNeighborhood = 9;
    if(searchPixels[4] == 0){
        for(int i = 0;i < 9;i++ ){
            if(i!=4){
                //std::cout<<searchPixels[i]<< " SearchPixels"<<std::endl;
                if((searchPixels[i] == 1 || searchPixels[i] == -1)){
                    ifEdge = true;
                }
            }//not the center pixel


        }
        if(ifEdge)
        {
            outputImg->data[currLocation] = 0;
        }
        else{
            outputImg->data[currLocation] = 255;
        }

    }
    else {
        outputImg->data[currLocation] = 255;
    }

}

void EdgeDetector::print_histogram(const std::string &fileName, int* freq) {
     FILE* histfile;
     histfile = fopen(fileName.c_str(), "w");

     if(histfile != nullptr){
         for (int i = 0; i < 256; i++) {
             fprintf(histfile, "%d\t%d\n", i, freq[i] );
         }
     }

}