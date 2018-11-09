//
// Created by Manasa Manohara on 3/16/18.

//
#include "Texture.h"
#include <iostream>
#include <map>
#include <vector>
#include<cmath>
#include<memory>

#include "opencv2/highgui.hpp"
using namespace cv;


Texture::Texture(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

Texture::~Texture(){
}



void Texture::tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize) {

    //convert to 2D arrays - 5x1  * 1x5

    int matrixDimension = matrixSize * matrixSize;

    double **filter_2_2DArrayAllocated = new double *[3];
    for (int i = 0; i < 3; i++) {
        filter_2_2DArrayAllocated[i] = new double[1];
        memset(filter_2_2DArrayAllocated[i], 0, 1 * sizeof(double));
    }
    double **filter_2DArray = filter_2_2DArrayAllocated;


    //get 2D array

    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < 1; col++) {
            filter_2DArray[row][col] = filter1[row];
        }
    }


    for (int mulrow = 0; mulrow < 3; mulrow++) {
        for (int mulcol = 0; mulcol < 3; mulcol++) {
            int currloc = (mulrow * 3) + mulcol;
            finalfilter[currloc] = finalfilter[currloc] + filter_2DArray[mulrow][0] * filter2[mulcol];

        }

    }
}

void Texture::texture_analysis(Image* inputImg,const std::string& fileName){
    int matrixSize = 3;
    int matrixDimension = matrixSize * matrixSize;


    //3x3 laws filters

    double L3[3] =  {(double)1/6 , (double)2/6 ,  (double)1/6 };
    double E3[3] =  {(double)-1/2 , 0 ,  (double)1/2 };
    double S3[3] =  {(double)-1/2 , (double)2/2 ,  (double)-1/2 };


    //pseudocode
    //get the DC reduced image
    //get the tensor product of filters
    //get the nearest neighbors of the double array
    //get the absolute sum and convert to unsgined char for the filtered image
    //get the energy of every filtered image


    //get tensor products of the 1D arrays
    //you need 9 Law's filter - so initialize 9 of them
    int neighborhood = 3;
    int imgDimension = inputImg->height * inputImg->width;
    //std::cout<<imgDimension<<std::endl;


    double reducedDCbeforeImg[imgDimension];
    memset(reducedDCbeforeImg,0,imgDimension * sizeof(double));
    // removeDC(inputImg,reducedDCbeforeImg);


    double reducedDCImg[imgDimension];
    memset(reducedDCImg,0,imgDimension * sizeof(double));
    //removeDC(inputImg,reducedDCImg);

    //convert image to double

    for(int i = 0;i < imgDimension; i++){
        reducedDCImg[i] = (double) inputImg->data[i];
    }

    int DCneighborhood = 21;
    nn_assign_DC(reducedDCImg, inputImg->width,  inputImg->height, DCneighborhood);


    double **filterD_1img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_1img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_1img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_1img = filterD_1img2DArrayAllocated;

    double **filterD_2img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_2img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_2img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_2img = filterD_2img2DArrayAllocated;

    double **filterD_3img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_3img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_3img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_3img = filterD_3img2DArrayAllocated;

    double **filterD_4img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_4img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_4img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_4img = filterD_4img2DArrayAllocated;

    double **filterD_5img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_5img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_5img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_5img = filterD_5img2DArrayAllocated;

    double **filterD_6img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_6img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_6img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_6img = filterD_6img2DArrayAllocated;

    double **filterD_7img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_7img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_7img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_7img = filterD_7img2DArrayAllocated;

    double **filterD_8img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_8img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_8img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_8img = filterD_8img2DArrayAllocated;


    double **filterD_9img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_9img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_9img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_9img = filterD_9img2DArrayAllocated;


    //repeat this 9 times

    //1 - EE
    double EE_filter1[matrixDimension];
    memset(EE_filter1,0,matrixDimension * sizeof(double));
    tensorProduct(E3, E3, EE_filter1, matrixSize);



    Image* filter1_img = new Image(inputImg->width, inputImg->height, 1);
    filter1_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter1_img, EE_filter1, filterD_1img);


    //2 - ES
    double ES_filter2[matrixDimension];
    memset(ES_filter2,0,matrixDimension * sizeof(double));
    tensorProduct(E3, S3, ES_filter2, matrixSize);


    Image* filter2_img = new Image(inputImg->width, inputImg->height, 1);
    filter2_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter2_img, ES_filter2,filterD_2img);

    //3 - EL
    double EL_filter3[matrixDimension];
    memset(EL_filter3,0,matrixDimension * sizeof(double));
    tensorProduct(E3, L3, EL_filter3, matrixSize);


    Image* filter3_img = new Image(inputImg->width, inputImg->height, 1);
    filter3_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter3_img, EL_filter3,filterD_3img);


    //4 - SE
    double SE_filter4[matrixDimension];
    memset(SE_filter4,0,matrixDimension * sizeof(double));
    tensorProduct(S3, E3, SE_filter4, matrixSize);


    Image* filter4_img = new Image(inputImg->width, inputImg->height, 1);
    filter4_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter4_img, SE_filter4,filterD_4img);

    //5 - SS
    double SS_filter5[matrixDimension];
    memset(SS_filter5,0,matrixDimension * sizeof(double));
    tensorProduct(S3, S3, SS_filter5, matrixSize);

    Image* filter5_img = new Image(inputImg->width, inputImg->height, 1);
    filter5_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter5_img, SS_filter5,filterD_5img);



    //6 - SL
    double SL_filter6[matrixDimension];
    memset(SL_filter6,0,matrixDimension * sizeof(double));
    tensorProduct(S3, L3, SL_filter6, matrixSize);

    Image* filter6_img = new Image(inputImg->width, inputImg->height, 1);
    filter6_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter6_img, SL_filter6,filterD_6img);


    //7 - LE
    double LE_filter7[matrixDimension];
    memset(LE_filter7,0,matrixDimension * sizeof(double));
    tensorProduct(L3, E3, LE_filter7, matrixSize);

    Image* filter7_img = new Image(inputImg->width, inputImg->height, 1);
    filter7_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter7_img, LE_filter7,filterD_7img);

    //8 - LS
    double LS_filter8[matrixDimension];
    memset(LS_filter8,0,matrixDimension * sizeof(double));
    tensorProduct(L3, S3, LS_filter8, matrixSize);

    Image* filter8_img = new Image(inputImg->width, inputImg->height, 1);
    filter8_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter8_img, LS_filter8,filterD_8img);

    //9 - LL
    double LL_filter9[matrixDimension];
    memset(LL_filter9,0,matrixDimension * sizeof(double));
    tensorProduct(L3, L3, LL_filter9, matrixSize);

    Image* filter9_img = new Image(inputImg->width, inputImg->height, 1);
    filter9_img->allocate();
    nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filter9_img, LL_filter9,filterD_9img);

    //get energy for each pixel

    int energyNeighborhood = 21;
    int filterNum = 0;

    int imgSize = inputImg->width * inputImg->height;
    int featuresNum = 9;

    double** energyArrayAllocated = new double* [imgSize];
    for(int i = 0; i < imgSize; i++){
        energyArrayAllocated[i] = new double[featuresNum];
        memset(energyArrayAllocated[i], 0, featuresNum * sizeof(double));
    }
    double** energyArray = energyArrayAllocated;

    calculate_energy(filterD_1img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_2img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_3img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_4img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_5img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_6img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_7img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_8img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_9img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);





    /////////////////////////////////////////////////////////////////////////////////normalize features by L3L3T////////////////////////////////////////////////////////////////

    /*   for(int i = 0; i <imgSize; i++){
           for(int j = 0 ; j < 9; j++){
               energyArray[i][j] = energyArray[i][j] / energyArray[i][8];
           }
       }*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    int classes = 6;
    kmeans( energyArray, classes, fileName);
}

double Texture::removeDC(double* searchPixels, int searchPixelsSize){

    double sumPixels = 0;
    for (int i = 0; i < searchPixelsSize; i++) {
        double currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            double value = searchPixels[i];
            sumPixels = sumPixels + (value );
            //std::cout<<i<<" , "<<sumPixels<< " , "<< searchPixels[i] << " , "<<std::endl;
        }
    }
    //  std::cout<<sumPixels<<std::endl;
    double meanIntensity = (sumPixels)/searchPixelsSize;
    //  std::cout<<meanIntensity<<std::endl;
    return meanIntensity;
}


void Texture::nn_assign_DC(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood){
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

            double searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(double));

            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    int newCol = 0;
                    int newRow = 0;
                    if (searchCol < 0 || searchCol >= imgWidth) {

                        if(searchCol < 0){
                            newCol = 0;
                        }
                        if(searchCol >= imgWidth){
                            newCol = imgWidth -  1;
                        }

                    }
                    else{
                        newCol = searchCol;
                    }

                    if (searchRow < 0 || searchRow >= imgHeight) {

                        if(searchRow < 0){
                            newRow = 0;
                        }
                        if(searchRow >= imgHeight){
                            newRow = imgHeight - 1;
                        }

                    }
                    else{
                        newRow = searchRow;
                    }


                    int searchPixelLocation = (newRow * imgWidth) + newCol;
                    searchPixels[searchPixelsCount] = reducedChannel[searchPixelLocation];
                    searchPixelsCount++;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    searchPixelsCount++;
                }
            }

            double medianIntensityNeighborhood = removeDC(searchPixels, searchPixelsSize);
            reducedChannel[currPixelLocation] = reducedChannel[currPixelLocation] - medianIntensityNeighborhood;


        }
    }

}


void Texture::nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, Image* assignedChannel, double* currentFilter, double** filterD_img){
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

            double searchPixels[searchPixelsSize];
            memset(searchPixels, 0, searchPixelsSize * sizeof(double));

            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {

                    int newCol = 0;
                    int newRow = 0;
                    if (searchCol < 0 || searchCol >= imgWidth) {

                        if(searchCol < 0){
                            newCol = 0;
                        }
                        if(searchCol >= imgWidth){
                            newCol = imgWidth -  1;
                        }

                    }
                    else{
                        newCol = searchCol;
                    }

                    if (searchRow < 0 || searchRow >= imgHeight) {
                        if(searchRow < 0){
                            newRow = 0;
                        }
                        if(searchRow >= imgHeight){
                            newRow = imgHeight - 1;
                        }
                    }
                    else{
                        newRow = searchRow;
                    }


                    int searchPixelLocation = (newRow * imgWidth) + newCol;
                    searchPixels[searchPixelsCount] = reducedChannel[searchPixelLocation];
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

            filterD_img[row][col] = medianIntensityNeighborhood;
            assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;

        }
    }

}


double Texture::laws_filter(double* searchPixels, int searchPixelsSize, double* inputFilter) {
    // We know that -800 is indicative of an unassigned pixel. Anything between 0-255 is a "good" pixel value.
    // Find the frequent non-zero pixel value and return it
    double sumPixels = 0;
    for (int i = 0; i < searchPixelsSize; i++) {
        double currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            double value = searchPixels[i] * inputFilter[i];
            sumPixels = sumPixels + (value );

        }
    }
    double newIntensity = (sumPixels);

    return newIntensity;
}

void Texture::calculate_energy(double** inputChannel, int neighborhood, double** energyArray, int filterNum, int imgSize, int imageWidth, int imageHeight){
    //get the neighborhood pixels again
    //calculate energy

    int searchStep = (neighborhood - 1)/2;

    for (int row = 0; row < imageHeight; row++) {
        for (int col = 0; col < imageWidth; col++) {
            int currPixelLocation = (row * imageWidth) + col;

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
                    if (searchCol < 0 || searchCol >= imageWidth) {

                        if(searchCol < 0){
                            newCol = 0;
                        }
                        if(searchCol >= imageWidth){
                            newCol = imageWidth - 1;
                        }

                    }
                    else{
                        newCol = searchCol;
                    }

                    if (searchRow < 0 || searchRow >= imageHeight) {
                        if(searchRow < 0){
                            newRow = 0;
                        }
                        if(searchRow >= imageHeight){
                            newRow = imageHeight - 1;
                        }

                    }
                    else{
                        newRow = searchRow;
                    }


                    int searchPixelLocation = (newRow * imageWidth) + newCol;
                    searchPixels[searchPixelsCount] = inputChannel[newRow][newCol];
                    searchPixelsCount++;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    searchPixelsCount++;
                }
            }

            double energyPixel = calc_energy_pixel(searchPixels, searchPixelsSize);

            energyArray[currPixelLocation][filterNum] = energyPixel;

        }
    }


}
double Texture::calc_energy_pixel(double* searchPixels, int searchPixelsSize){
    double energy = 0;
    double sqrEnergy = 0;

    double sumPixels = 0;
    for (int i = 0; i < searchPixelsSize; i++) {
        double currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            double value = searchPixels[i] ;
            value = pow(value,2);
            sumPixels = sumPixels + (value );
        }
    }

    double average = sumPixels / searchPixelsSize;
    return average;
}


void Texture::kmeans( double** featureArray, int classes,const std::string& fileName ){

    std::cout<< "Inside k means"<<std::endl;
    int classColors[6] = {0, 51, 102, 153, 204, 255};
    int classWidth = 9;
    int classHeight = 4;



    double **means_2DArrayAllocated = new double *[6];
    for (int i = 0; i < 6; i++) {
        means_2DArrayAllocated[i] = new double[9];
        memset(means_2DArrayAllocated[i], 0, 9 * sizeof(double));
    }
    double **meansArray = means_2DArrayAllocated;

    int imgSize = inImg->height * inImg->width;

    int currentClass[imgSize];
    memset(currentClass,0,imgSize*sizeof(int));



    for(int j = 0; j < 9; j ++){
        meansArray[0][j] = featureArray[21095][j]; //299,67 = 80699

        meansArray[1][j] = featureArray[24895][j]; // 97C, 89r = 53497
        meansArray[2][j] = featureArray[55093][j];//472, 84 = 50872
        meansArray[3][j] = featureArray[132891][j];//83,388R = 232883
        meansArray[4][j] = featureArray[222680][j];//510,378 = 227310
        meansArray[5][j] = featureArray[218883][j];//295, 231 = 138895
    }

    Image *classImg = new Image(inImg->width, inImg->height, 1);
    classImg->allocate();

//get the minimum distance between the inputs and the class means

    for(int iteration = 0; iteration < 20; iteration ++){
        std::cout<<"Iteration : "<<iteration<<std::endl;

        for(int currentInput = 0; currentInput < imgSize; currentInput ++){
            double minDist = 0;
            for(int currentMean = 0; currentMean < 6; currentMean++){

                double sumDist = 0;

                for(int currentOrd = 0; currentOrd < 9; currentOrd++){

                    double difference = featureArray[currentInput][currentOrd] - meansArray[currentMean][currentOrd];
                    double sqrdDiff = pow(difference,2);
                    sumDist = sumDist + sqrdDiff;
                }
                sumDist = sqrt(sumDist);

                if(currentMean == 0){
                    minDist = sumDist;
                    currentClass[currentInput] = currentMean;
                    int value = classColors[currentMean];
                    classImg->data[currentInput] = (unsigned char) value;
                }
                else{
                    if(sumDist < minDist ){
                        minDist = sumDist;
                        currentClass[currentInput] = currentMean;
                        int value = classColors[currentMean];
                        classImg->data[currentInput] = (unsigned char) value;
                    }
                    else{
                        minDist = minDist;
                    }

                }


            }

        }

        ///make alll means zero

        for(int i = 0 ; i < 6; i ++){
            for(int j = 0 ; j < 9 ; j ++){
                meansArray[i][j] = 0;
            }
        }
        //calculate new means
        for(int numClasses = 0; numClasses < 6; numClasses++){
            double newMeans = 0;
            //check how many claases
            int count = 0;

            for (int input = 0; input < 9; input++) {
                double sumMeans = 0;
                for(int col = 0; col < imgSize; col++) {
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

        classImg->name = ("Class img");
        classImg->print_image(fileName, classImg);

    }
}


