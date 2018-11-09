//
// Created by Manasa Manohara on 3/13/18.
//


#include "Texture.h"
#include <iostream>
#include<cmath>


Texture::Texture(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

Texture::~Texture(){
}


void Texture::tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize) {

    //convert to 2D arrays - 5x1  * 1x5

    int matrixDimension = matrixSize * matrixSize;

    double **filter_2_2DArrayAllocated = new double *[5];
    for (int i = 0; i < 5; i++) {
        filter_2_2DArrayAllocated[i] = new double[1];
        memset(filter_2_2DArrayAllocated[i], 0, 1 * sizeof(double));
    }
    double **filter_2DArray = filter_2_2DArrayAllocated;




    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < 1; col++) {
            filter_2DArray[row][col] = filter1[row];
        }
    }


    for (int mulrow = 0; mulrow < 5; mulrow++) {
        for (int mulcol = 0; mulcol < 5; mulcol++) {
            int currloc = (mulrow * 5) + mulcol;
            finalfilter[currloc] = finalfilter[currloc] + filter_2DArray[mulrow][0] * filter2[mulcol];

        }

    }
}

void Texture::texture_analysis(Image* inputImg,double* imgFilter){
    int matrixSize = 5;
    int matrixDimension = matrixSize * matrixSize;

    //initialize 1D arrays
    double E5[5] =  {(double)-1/6 , (double)-2/6 , 0 , (double) 2/6  , (double)1/6 };
    double S5[5] =  {(double)-1/4, 0 , (double) 2/4, 0, (double)-1/4};
    double W5[5] =  {(double)-1/6, (double)2/6, 0 , (double)-2/6 , (double)1/6};

  //double E5[5] = {-1, -2, 0, 2, 1};
   // double S5[5] = {-1,0,2,0,-1};

    //pseudocode
    //get the DC reduced image
    //get the tensor product of filters
    //get the nearest neighbors of the double array
    //get the absolute sum and convert to unsgined char for the filtered image
    //get the energy of every filtered image


    //get tensor products of the 1D arrays
    //you need 9 Law's filter - so initialize 9 of them
    int neighborhood = 5;
    int imgDimension = inputImg->height * inputImg->width;
    double reducedDCImg[imgDimension];
    memset(reducedDCImg,0,imgDimension * sizeof(double));
    removeDC(inputImg,reducedDCImg);


    //repeat this 9 times


    //1 - EE
    double EE_filter1[matrixDimension];
    memset(EE_filter1,0,matrixDimension * sizeof(double));
    tensorProduct(E5, E5, EE_filter1, matrixSize);

    Image* filter1_img = new Image(inputImg->width, inputImg->height, 1);
    filter1_img->allocate();
    imgFilter[0] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter1_img, EE_filter1);
    //std::cout<<"Imgae filter here : "<< imgFilter[0]<<std::endl;

    //2 - ES
    double ES_filter2[matrixDimension];
    memset(ES_filter2,0,matrixDimension * sizeof(double));
    tensorProduct(E5, S5, ES_filter2, matrixSize);

    Image* filter2_img = new Image(inputImg->width, inputImg->height, 1);
    filter2_img->allocate();
    imgFilter[1] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter2_img, ES_filter2);
    // std::cout<<"Imgae filter here : "<< imgFilter[1]<<std::endl;

    //3 - EW
    double EW_filter3[matrixDimension];
    memset(EW_filter3,0,matrixDimension * sizeof(double));
    tensorProduct(E5, W5, EW_filter3, matrixSize);

    Image* filter3_img = new Image(inputImg->width, inputImg->height, 1);
    filter3_img->allocate();
    imgFilter[2] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter3_img, EW_filter3);
     //std::cout<<"Imgae filter here : "<< imgFilter[2]<<std::endl;

    //4 - SE
    double SE_filter4[matrixDimension];
    memset(SE_filter4,0,matrixDimension * sizeof(double));
    tensorProduct(S5, E5, SE_filter4, matrixSize);


    Image* filter4_img = new Image(inputImg->width, inputImg->height, 1);
    filter4_img->allocate();
    imgFilter[3] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter4_img, SE_filter4);
     //std::cout<<"Imgae filter here : "<< imgFilter[3]<<std::endl;

    //5 - SS
    double SS_filter5[matrixDimension];
    memset(SS_filter5,0,matrixDimension * sizeof(double));
    tensorProduct(S5, S5, SS_filter5, matrixSize);


    Image* filter5_img = new Image(inputImg->width, inputImg->height, 1);
    filter5_img->allocate();
    imgFilter[4] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter5_img, SS_filter5);
    //std::cout<<"Imgae filter here : "<< imgFilter[4]<<std::endl;


    //6 - SW
    double SW_filter6[matrixDimension];
    memset(SW_filter6,0,matrixDimension * sizeof(double));
    tensorProduct(S5, W5, SW_filter6, matrixSize);

    Image* filter6_img = new Image(inputImg->width, inputImg->height, 1);
    filter6_img->allocate();
    imgFilter[5] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter6_img, SW_filter6);
    //std::cout<<"Imgae filter here : "<< imgFilter[5]<<std::endl;

    //7 - WE
    double WE_filter7[matrixDimension];
    memset(WE_filter7,0,matrixDimension * sizeof(double));
    tensorProduct(W5, E5, WE_filter7, matrixSize);

    Image* filter7_img = new Image(inputImg->width, inputImg->height, 1);
    filter7_img->allocate();
    imgFilter[6] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter7_img, WE_filter7);
   // std::cout<<"Imgae filter here : "<< imgFilter[6]<<std::endl;

    //8 - WS
    double WS_filter8[matrixDimension];
    memset(WS_filter8,0,matrixDimension * sizeof(double));
    tensorProduct(W5, S5, WS_filter8, matrixSize);

    Image* filter8_img = new Image(inputImg->width, inputImg->height, 1);
    filter8_img->allocate();
    imgFilter[7] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter8_img, WS_filter8);
   // std::cout<<"Imgae filter here : "<< imgFilter[7]<<std::endl;

    //9 - WW
    double WW_filter9[matrixDimension];
    memset(WW_filter9,0,matrixDimension * sizeof(double));
    tensorProduct(W5, W5, WW_filter9, matrixSize);

    Image* filter9_img = new Image(inputImg->width, inputImg->height, 1);
    filter9_img->allocate();
    imgFilter[8] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter9_img, WW_filter9);
    //std::cout<<"Imgae filter here : "<< imgFilter[8]<<std::endl;






}

void Texture::removeDC(Image* inputImg, double* reducedDCImg){
    int imgSize = inputImg->width * inputImg->height;
    double sumIntensities = 0;
    for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
            int currentPixel = (row * inputImg->width) + col;
            int currLocation = currentPixel * 1;
            sumIntensities = sumIntensities + (double)inputImg->data[currLocation];
        }
    }

    double meanIntensity = (double) sumIntensities / (double)imgSize;

    for(int row = 0; row < inputImg->height; row++){
        for(int col = 0; col < inputImg->width; col++){
            int currentPixel = (row * inputImg->width) + col;
            int currLocation = currentPixel * 1;
            reducedDCImg[currLocation] = (inputImg->data[currLocation] - meanIntensity);
        }
    }
}

double Texture::nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, Image* assignedChannel, double* currentFilter){
    int searchStep = (neighborhood - 1)/2;

    double energy = 0;
    double sqrEnergy = 0;
    for (int row = 0; row < imgHeight; row++) {
        for (int col = 0; col < imgWidth; col++) {
            int currPixelLocation = (row * imgWidth) + col;

            int searchRowStart = row - searchStep;
            int searchRowEnd = row + searchStep;

            int searchColStart = col - searchStep;
            int searchColEnd = col + searchStep;

            int searchPixelsCount = 0;
            int searchPixelsSize = neighborhood * neighborhood;

            double searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(double));
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

            double medianIntensityNeighborhood = laws_filter(searchPixels, searchPixelsSize, currentFilter);
           // std::cout<<currPixelLocation << " , "<<medianIntensityNeighborhood<< std::endl;
            assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;
            energy = medianIntensityNeighborhood;;
            energy = pow(energy,2);
            sqrEnergy = sqrEnergy + energy;


        }
    }


double filterEnergy = sqrEnergy / (double)(imgHeight * imgWidth);
   // std::cout<<"Filter energy = "<<filterEnergy<<std::endl;
    return filterEnergy;
}


double Texture::laws_filter(double* searchPixels, int searchPixelsSize, double* inputFilter) {
    // We know that -1 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it
 double sumPixels = 0;
    for (int i = 0; i < searchPixelsSize; i++) {
        double currPixelValue = searchPixels[i];

            double value = searchPixels[i] * inputFilter[i];
            sumPixels = sumPixels + (value );
           // std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;

    }
    double newIntensity = (sumPixels);
    return newIntensity;
}


void Texture::kmeans( double** featureArray, int classes, double** meansArray){

    int classWidth = 9;
    int classHeight = 4;



    int currentClass[12];
    memset(currentClass,0,12*sizeof(int));



//get the minimum distance between the inputs and the class means

    int predictedClasses[12];
    memset(predictedClasses, 0, 12 * sizeof(int));
    for(int iteration = 0; iteration < 4; iteration ++){
        std::cout<<"Iteration : "<<iteration<<std::endl;
        for(int currentInput = 0; currentInput < 12; currentInput ++){
            double minDist = 0;
            for(int currentMean = 0; currentMean < 4; currentMean++){

                    double sumDist = 0;

                    for(int currentOrd = 0; currentOrd < 9; currentOrd++){

                        double difference = featureArray[currentInput][currentOrd] - meansArray[currentMean][currentOrd];
                         //std::cout<<currentInput<<" , "<< currentOrd<< " : " <<featureArray[currentInput][currentOrd] << " , "<<meansArray[currentMean][currentOrd]<<std::endl;
                        double sqrdDiff = pow(difference,2);
                        sumDist = sumDist + sqrdDiff;



                    }
                sumDist = sqrt(sumDist);
                    //  std::cout<<"current mean and distances : "<<currentMean<< " , "<< sumDist<<std::endl;
                    if(currentMean == 0){
                        minDist = sumDist;
                        currentClass[currentInput] = currentMean;
                    }
                    else{
                        if(sumDist < minDist ){
                            minDist = sumDist;
                            currentClass[currentInput] = currentMean;
                        }
                        else{
                            minDist = minDist;
                        }

                    }


            }
        std::cout<<"Minimum dist with class : "<<currentInput<<" , "<< minDist << " , "<<currentClass[currentInput]<<std::endl;

        }

    ///make alll means zero

    for(int i = 0 ; i < 4; i ++){
        for(int j = 0 ; j < 9 ; j ++){
            meansArray[i][j] = 0;
        }
    }
    //calculate new means
    for(int numClasses = 0; numClasses < 4; numClasses++){
        double newMeans = 0;
        //check how many claases
        int count = 0;

        for (int input = 0; input < 9; input++) {
            double sumMeans = 0;
            for(int col = 0; col < 12; col++) {
                    if (currentClass[col] == numClasses) {
                        if (input == 0) {
                            count++;
                        }

                        //assign new means
                        sumMeans = sumMeans + featureArray[col][input];
                        newMeans = sumMeans / count;
                        meansArray[numClasses][input] = newMeans;
                    }

                }
            }


    }

    }

}

