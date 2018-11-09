//
// Created by Manasa Manohara on 3/22/18.
//

//
// Created by Manasa Manohara on 3/16/18.
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
#include "Texture.h"
#include <iostream>
#include <map>
#include <vector>
#include<cmath>
#include<memory>

#include "opencv2/highgui.hpp"
#include<iostream>
#include <fstream>
using namespace cv;


Texture::Texture(Image& image, bool printDebug) {
    this->inImg = &image;
    this->printDebug = printDebug;
}

Texture::~Texture(){
}



void Texture::combine_channels(Image * redReducedChannel, Image * greenReducedChannel, Image * blueReducedChannel, Image *  reducedAllChannels, int imgWidth, int imgHeight){
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

void Texture::tensorProduct(const double* filter1, double* filter2, double* finalfilter, int matrixSize) {

    //convert to 2D arrays - 5x1  * 1x5

    int matrixDimension = matrixSize * matrixSize;

    double **filter_2_2DArrayAllocated = new double *[5];
    for (int i = 0; i < 5; i++) {
        filter_2_2DArrayAllocated[i] = new double[5];
        memset(filter_2_2DArrayAllocated[i], 0, 5 * sizeof(double));
    }
    double **filter_2DArray = filter_2_2DArrayAllocated;


    //get 2D array

    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < 5; col++) {
            filter_2DArray[row][col] = filter1[row];
        }
    }


    for (int mulrow = 0; mulrow < 5; mulrow++) {
        for (int mulcol = 0; mulcol < 5; mulcol++) {
            int currloc = (mulrow * 5) + mulcol;
            finalfilter[currloc] = finalfilter[currloc] + filter_2DArray[mulrow][0] * filter2[mulcol];
            //std::cout << "Individual : " << filter_2DArray[mulrow][0] << " , " <<filter2[mulcol]<<", "<< mulrow << " , " << mulcol << std::endl;
            // std::cout << "Here : " << finalfilter[currloc] << " , " << currloc<< std::endl;

        }

    }
}

void Texture::texture_analysis(Image* inputImg,double* imgFilter,const std::string &fileName, const std::string &fileNamePca,  const std::string &fileNameOutput){
    int matrixSize = 5;
    int matrixDimension = matrixSize * matrixSize;

    //initialize 1D arrays
     double E5[5] =  {(double)-1/6 , (double)-2/6 , 0 , (double) 2/6  , (double)1/6 };
     double S5[5] =  {(double)-1/4, 0 , (double) 2/4, 0, (double)-1/4};
     double W5[5] =  {(double)-1/6, (double)2/6, 0 , (double)-2/6 , (double)1/6};
     double L5[5] =  {(double)1/16, (double) 4/16 , (double) 6/16, (double) 4/16, (double)1/16};
     double R5[5] =  {(double)1/16, (double) -4/16 , (double) 6/16, (double) -4/16, (double)1/16};


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

    double **filterD_10img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_10img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_10img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_10img = filterD_10img2DArrayAllocated;

    double **filterD_11img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_11img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_11img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_11img = filterD_11img2DArrayAllocated;

    double **filterD_12img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_12img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_12img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_12img = filterD_12img2DArrayAllocated;

    double **filterD_13img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_13img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_13img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_13img = filterD_13img2DArrayAllocated;

    double **filterD_14img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_14img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_14img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_14img = filterD_14img2DArrayAllocated;

    double **filterD_15img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_15img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_15img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_15img = filterD_15img2DArrayAllocated;

    double **filterD_16img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_16img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_16img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_16img = filterD_16img2DArrayAllocated;

    double **filterD_17img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_17img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_17img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_17img = filterD_17img2DArrayAllocated;

    double **filterD_18img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_18img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_18img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_18img = filterD_18img2DArrayAllocated;

    double **filterD_19img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_19img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_19img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_19img = filterD_19img2DArrayAllocated;

    double **filterD_20img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_20img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_20img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_20img = filterD_20img2DArrayAllocated;

    double **filterD_21img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_21img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_21img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_21img = filterD_21img2DArrayAllocated;

    double **filterD_22img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_22img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_22img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_22img = filterD_22img2DArrayAllocated;

    double **filterD_23img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_23img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_20img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_23img = filterD_23img2DArrayAllocated;

    double **filterD_24img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_24img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_24img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_24img = filterD_24img2DArrayAllocated;

    double **filterD_25img2DArrayAllocated = new double *[inputImg->height];
    for (int i = 0; i < inputImg->height; i++) {
        filterD_25img2DArrayAllocated[i] = new double[inputImg->width];
        memset(filterD_25img2DArrayAllocated[i], 0, inputImg->width * sizeof(double));
    }
    double **filterD_25img = filterD_25img2DArrayAllocated;

    //

    //1 - LL
    double LL_filter1[matrixDimension];
    memset(LL_filter1,0,matrixDimension * sizeof(double));
    tensorProduct(L5, L5, LL_filter1, matrixSize);

    imgFilter[0] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_1img, LL_filter1);

    std::cout<<"Imgae filter here : "<< imgFilter[0]<<std::endl;
    //2-LE

  
    double LE_filter2[matrixDimension];
    memset(LE_filter2,0,matrixDimension * sizeof(double));
    tensorProduct(L5, E5, LE_filter2, matrixSize);

    imgFilter[1] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_2img, LE_filter2);
    std::cout<<"Imgae filter here : "<< imgFilter[1]<<std::endl;
    //3 - LS


    double LS_filter3[matrixDimension];
    memset(LS_filter3,0,matrixDimension * sizeof(double));
    tensorProduct(L5, S5, LS_filter3, matrixSize);

    imgFilter[2] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_3img, LS_filter3);
    std::cout<<"Imgae filter here : "<< imgFilter[2]<<std::endl;
    
    //4 - LW

    double LW_filter4[matrixDimension];
    memset(LW_filter4,0,matrixDimension * sizeof(double));
    tensorProduct(L5, W5, LW_filter4, matrixSize);


    imgFilter[3] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_4img, LW_filter4);
    std::cout<<"Imgae filter here : "<< imgFilter[3]<<std::endl;
    
    //5 - LR

    double LR_filter5[matrixDimension];
    memset(LR_filter5,0,matrixDimension * sizeof(double));
    tensorProduct(L5, R5, LR_filter5, matrixSize);

    imgFilter[4] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_5img, LR_filter5);

    std::cout<<"Imgae filter here : "<< imgFilter[4]<<std::endl;
    //6 -EL
    double EL_filter6[matrixDimension];
    memset(EL_filter6,0,matrixDimension * sizeof(double));
    tensorProduct(E5, L5, EL_filter6, matrixSize);

    imgFilter[5] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_6img, EL_filter6);

    std::cout<<"Imgae filter here : "<< imgFilter[5]<<std::endl;

    //7 - EE
    double EE_filter7[matrixDimension];
    memset(EE_filter7,0,matrixDimension * sizeof(double));
    tensorProduct(E5, E5, EE_filter7, matrixSize);


    imgFilter[6] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_7img, EE_filter7);
    std::cout<<"Imgae filter here : "<< imgFilter[6]<<std::endl;

    //8 - ES
    double ES_filter8[matrixDimension];
    memset(ES_filter8,0,matrixDimension * sizeof(double));
    tensorProduct(E5, S5, ES_filter8, matrixSize);

    imgFilter[7] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_8img, ES_filter8);
     std::cout<<"Imgae filter here : "<< imgFilter[7]<<std::endl;

    //9 - EW
    double EW_filter9[matrixDimension];
    memset(EW_filter9,0,matrixDimension * sizeof(double));
    tensorProduct(E5, W5, EW_filter9, matrixSize);

    imgFilter[8] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_9img, EW_filter9);
    std::cout<<"Imgae filter here : "<< imgFilter[8]<<std::endl;

    //10-ER
    double ER_filter10[matrixDimension];
    memset(ER_filter10,0,matrixDimension * sizeof(double));
    tensorProduct(E5, R5, ER_filter10, matrixSize);

    imgFilter[9] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_10img, ER_filter10);
    std::cout<<"Imgae filter here : "<< imgFilter[9]<<std::endl;
    
    //11 - SL 
    double SL_filter11[matrixDimension];
    memset(SL_filter11,0,matrixDimension * sizeof(double));
    tensorProduct(S5, L5, SL_filter11, matrixSize);

    imgFilter[10] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_11img, SL_filter11);
    std::cout<<"Imgae filter here : "<< imgFilter[10]<<std::endl;
    
    
    //12 - SE
    double SE_filter12[matrixDimension];
    memset(SE_filter12,0,matrixDimension * sizeof(double));
    tensorProduct(S5, E5, SE_filter12, matrixSize);
//    for(int i = 0; i < 25; i ++){
//        std::cout<< " Tensor product filter : "<< i << "  : "<<SE_filter4[i]<<std::endl;
//    }


    imgFilter[11] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_12img, SE_filter12);
    std::cout<<"Imgae filter here : "<< imgFilter[11]<<std::endl;

    //13 - SS
    double SS_filter13[matrixDimension];
    memset(SS_filter13,0,matrixDimension * sizeof(double));
    tensorProduct(S5, S5, SS_filter13, matrixSize);

    imgFilter[12] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_13img, SS_filter13);
    std::cout<<"Imgae filter here : "<< imgFilter[12]<<std::endl;


    //14 - SW
    double SW_filter14[matrixDimension];
    memset(SW_filter14,0,matrixDimension * sizeof(double));
    tensorProduct(S5, W5, SW_filter14, matrixSize);


    imgFilter[13] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_14img, SW_filter14);
    std::cout<<"Imgae filter here : "<< imgFilter[13]<<std::endl;

    //15 - SR
    double SR_filter15[matrixDimension];
    memset(SR_filter15,0,matrixDimension * sizeof(double));
    tensorProduct(S5, R5, SR_filter15, matrixSize);


    imgFilter[14] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_15img, SR_filter15);
    std::cout<<"Imgae filter here : "<< imgFilter[14]<<std::endl;

    //16 - WL
    double WL_filter16[matrixDimension];
    memset(WL_filter16,0,matrixDimension * sizeof(double));
    tensorProduct(W5, L5, WL_filter16, matrixSize);

    imgFilter[15] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_16img, WL_filter16);
    std::cout<<"Imgae filter here : "<< imgFilter[15]<<std::endl;
    
    //17 - WE
    double WE_filter17[matrixDimension];
    memset(WE_filter17,0,matrixDimension * sizeof(double));
    tensorProduct(W5, E5, WE_filter17, matrixSize);

    imgFilter[16] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_17img, WE_filter17);
    std::cout<<"Imgae filter here : "<< imgFilter[16]<<std::endl;
    //18 - WS
    double WS_filter18[matrixDimension];
    memset(WS_filter18,0,matrixDimension * sizeof(double));
    tensorProduct(W5, S5, WS_filter18, matrixSize);


    imgFilter[17] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_18img, WS_filter18);
    std::cout<<"Imgae filter here : "<< imgFilter[17]<<std::endl;
    //19 - WW
    double WW_filter19[matrixDimension];
    memset(WW_filter19,0,matrixDimension * sizeof(double));
    tensorProduct(W5, W5, WW_filter19, matrixSize);

    imgFilter[18] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_19img, WW_filter19);
    std::cout<<"Imgae filter here : "<< imgFilter[18]<<std::endl;


    //20 - WR
    double WR_filter20[matrixDimension];
    memset(WR_filter20,0,matrixDimension * sizeof(double));
    tensorProduct(W5, R5, WR_filter20, matrixSize);


    imgFilter[19] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_20img, WR_filter20);
    std::cout<<"Imgae filter here : "<< imgFilter[19]<<std::endl;
    
    //21- RL
    double RL_filter21[matrixDimension];
    memset(RL_filter21,0,matrixDimension * sizeof(double));
    tensorProduct(R5, L5, RL_filter21, matrixSize);


    imgFilter[20] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_21img, RL_filter21);
    std::cout<<"Imgae filter here : "<< imgFilter[20]<<std::endl;
    //22 - RE
    double RE_filter21[matrixDimension];
    memset(RE_filter21,0,matrixDimension * sizeof(double));
    tensorProduct(R5, E5, RE_filter21, matrixSize);

    imgFilter[21] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_22img, RE_filter21);
    std::cout<<"Imgae filter here : "<< imgFilter[21]<<std::endl;
    //23 - RS
    double RS_filter23[matrixDimension];
    memset(RS_filter23,0,matrixDimension * sizeof(double));
    tensorProduct(R5, S5, RS_filter23, matrixSize);


    imgFilter[22] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_23img, RS_filter23);
    std::cout<<"Imgae filter here : "<< imgFilter[22]<<std::endl;
    //24 - RW
    double RW_filter24[matrixDimension];
    memset(RW_filter24,0,matrixDimension * sizeof(double));
    tensorProduct(R5, W5, RW_filter24, matrixSize);


    imgFilter[23] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_24img, RW_filter24);
    std::cout<<"Imgae filter here : "<< imgFilter[23]<<std::endl;
    //25 - RR 
    double RR_filter25[matrixDimension];
    memset(RR_filter25,0,matrixDimension * sizeof(double));
    tensorProduct(R5, R5, RR_filter25, matrixSize);


    imgFilter[24] = nn_assign(reducedDCImg, inputImg->width, inputImg->height, neighborhood, filterD_25img, RR_filter25);
    std::cout<<"Imgae filter here : "<< imgFilter[24]<<std::endl;

    std::cout<<"Imgae filter here  ENDS ENDS ENDS "<<std::endl;


    /*  for(int row =0; row < inputImg->height; row++){
          for(int col = 0; col < inputImg->width; col++){
              int currLocation = (row * inputImg->width) + col;
              std::cout<<(int)filter1_img->data[currLocation]<<std::endl;
          }
      }*/


    //claculate energy for individual pixels
    int energyNeighborhood = 19;
    int filterNum = 0;

    int imgSize = inputImg->width * inputImg->height;
    int featuresNum = 25;

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


    //

    filterNum++;
    calculate_energy(filterD_10img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_11img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_12img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_13img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_14img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_15img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_16img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_17img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_18img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_19img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_20img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_21img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_22img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_23img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_24img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);
    filterNum++;
    calculate_energy(filterD_25img, energyNeighborhood, energyArray, filterNum,imgSize,inputImg->width, inputImg->height);






    //normalize features
    // normalize_feature(energyArray,energyArray,imgSize);


//print values to a text file

    FILE* histfile;
    histfile = fopen(fileName.c_str(), "w");

    if(histfile != nullptr){
        for (int i = 0; i < imgSize; i++) {
            for(int j = 0; j< 25; j++){
                fprintf(histfile, "%f\t", energyArray[i][j]);

            }
            fprintf(histfile, "\n");
        }
    }


    //read pca file





    //read from MATLAB



    double x = 0 ;
    std::ifstream inFile;
    inFile.open(fileNamePca);
    int pca_dim = 600*450*5;
    int pca_height = 600*450;
    int pca_width = 5;

   // double pcaArray[pca_dim];
    //memset(pcaArray, 0, pca_dim * sizeof(double));

    double **pcaArrayAllocated = new double *[pca_height];
    for (int i = 0; i < pca_height; i++) {
        pcaArrayAllocated[i] = new double[pca_width];
        memset(pcaArrayAllocated[i], 0, pca_width * sizeof(double));
    }
    double **pcaArray = pcaArrayAllocated;


    if(!inFile){
        std::cout<<"Unable to open file"<<std::endl;
        exit(1);
    }
    int row = 0;
    int col = 0;
    while(inFile >> x){
                pcaArray[row][col] = x;
                col++;
        if(col == pca_width)
        {
            col = 0;
            row++;
        }
    }
    inFile.close();



       int classes = 6;
    kmeans(  pcaArray, classes, fileNameOutput );
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

            double medianIntensityNeighborhood = removeDC(searchPixels, searchPixelsSize);
            reducedChannel[currPixelLocation] = reducedChannel[currPixelLocation] - medianIntensityNeighborhood;
           // std::cout<<reducedChannel[currPixelLocation]<< " "<<currPixelLocation<<std::endl;


        }
    }

//  I AM AWSEOME PAWSOME, you better believe it!! yeah!
}


double Texture::nn_assign(double* reducedChannel, int imgWidth, int imgHeight, int neighborhood, double** filterD_img, double* currentFilter){
    int searchStep = (neighborhood - 1)/2;
    std::cout<<imgWidth<<" ; "<<imgHeight<<std::endl;
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
            filterD_img[row][col] = medianIntensityNeighborhood;
            // std::cout<<"Filtered Image  : "<<filterD_img[row][col]<< " : " <<row<<" , "<<col<<std::endl;
           // assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;

            energy = medianIntensityNeighborhood;;
            energy = pow(energy,2);
            sqrEnergy = sqrEnergy + energy;
        }
    }


    double filterEnergy = sqrEnergy / (imgHeight * imgWidth);
    // std::cout<<"Filter energy = "<<filterEnergy<<std::endl;
    return filterEnergy;

    //  I AM AWSEOME PAWSOME, you better believe it!! yeah!
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
            //  std::cout<<i<<" , "<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }
    // std::cout<<sumPixels<<std::endl;
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
                        //searchPixels[searchPixelsCount] = -800;
                        //searchPixelsCount++;

                        if(searchCol < 0){
                            newCol = 0;
                        }
                        if(searchCol >= imageWidth){
                            newCol = imageWidth - 1;
                        }
                        // searchPixelsCount++;
                        //continue;
                    }
                    else{
                        newCol = searchCol;
                    }

                    if (searchRow < 0 || searchRow >= imageHeight) {
                        // searchPixels[searchPixelsCount] = -800;
                        //searchPixelsCount++;

                        if(searchRow < 0){
                            newRow = 0;
                        }
                        if(searchRow >= imageHeight){
                            newRow = imageHeight - 1;
                        }
                        // searchPixelsCount++;
                        // continue;
                    }
                    else{
                        newRow = searchRow;
                    }


                    int searchPixelLocation = (newRow * imageWidth) + newCol;
                    searchPixels[searchPixelsCount] = inputChannel[newRow][newCol];
                    // std::cout<< searchRow<<", "<<searchCol<< " : "<<reducedChannel[searchPixelLocation]<<std::endl;
                    searchPixelsCount++;
                }
            }

            searchPixelsCount = 0;
            for (int searchRow = searchRowStart; searchRow <= searchRowEnd; searchRow++) {
                for (int searchCol = searchColStart; searchCol <= searchColEnd; searchCol++) {
                    searchPixelsCount++;
                }
            }

            double energyPixel = calc_energy_pixel(searchPixels, searchPixelsSize, filterNum, imgSize);

            energyArray[currPixelLocation][filterNum] = energyPixel;

            // std::cout<<currPixelLocation << " , "<<medianIntensityNeighborhood<< std::endl;
            // assignedChannel->data[currPixelLocation] = (unsigned char) medianIntensityNeighborhood;




        }
    }


}
double Texture::calc_energy_pixel(double* searchPixels, int searchPixelsSize, int currentFilter, int imgSize){
    double energy = 0;
    double sqrEnergy = 0;

    double sumPixels = 0;
    for (int i = 0; i < searchPixelsSize; i++) {
        double currPixelValue = searchPixels[i];
        if (currPixelValue != -800) {
            double value = searchPixels[i] ;
            value = pow(value,2);
            sumPixels = sumPixels + (value );
            // std::cout<<sumPixels<< " , "<< searchPixels[i] << " , "<< inputFilter[i]<<std::endl;
        }
    }

    double average = sumPixels / searchPixelsSize;

    return average;
}


void Texture::kmeans(double**featureArray, int classes,const std::string &fileNameOutput){

    std::cout<< "Inside k means"<<std::endl;
    int classColors[6] = {0, 51, 102, 153, 204, 255};
    int classWidth = 9;
    int classHeight = 4;

    //  std::cout<<featureArray[18300][4]<<std::endl;

    double **means_2DArrayAllocated = new double *[6];
    for (int i = 0; i < 6; i++) {
        means_2DArrayAllocated[i] = new double[5];
        memset(means_2DArrayAllocated[i], 0, 5 * sizeof(double));
    }
    double **meansArray = means_2DArrayAllocated;

    int imgSize = inImg->height * inImg->width;

    int currentClass[imgSize];
    memset(currentClass,0,imgSize*sizeof(int));

    //choose means properly
    /*for(int i = 0; i < 6; i ++){
        for(int j = 0; j < 9; j++){
            meansArray[i][j] = rand() % 1;
        }
    }
*/
    for(int j = 0; j < 5; j ++){
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

                for(int currentOrd = 0; currentOrd < 5; currentOrd++){

                    double difference = featureArray[currentInput][currentOrd] - meansArray[currentMean][currentOrd];
                    // std::cout<<currentInput<<" , "<< currentOrd<< " : " <<featureArray[currentInput][currentOrd] << " , "<<meansArray[currentMean][currentOrd]<<" : " <<difference<<std::endl;
                    double sqrdDiff = pow(difference,2);
                    //std::cout<<sqrdDiff<<std::endl;
                    sumDist = sumDist + sqrdDiff;
                    //std::cout<<sumDist<<std::endl;



                }
                sumDist = sqrt(sumDist);
                // std::cout<<sumDist<<std::endl;
                //   std::cout<<"current mean and distances : "<<currentMean<< " , "<< sumDist<<std::endl;
                if(currentMean == 0){
                    minDist = sumDist;
                    currentClass[currentInput] = currentMean;
                    int value = classColors[currentMean];
                    //std::cout<<"If : Value : "<<value<<", " <<currentInput<<" , " <<currentMean<<std::endl;
                    classImg->data[currentInput] = (unsigned char) value;
                }
                else{
                    if(sumDist < minDist ){
                        minDist = sumDist;
                        currentClass[currentInput] = currentMean;
                        int value = classColors[currentMean];
                        //std::cout<<"Else : Value : "<<value<<", " <<currentInput<<" , " <<currentMean<<std::endl;
                        classImg->data[currentInput] = (unsigned char) value;
                    }
                    else{
                        minDist = minDist;
                    }

                }


            }
            // std::cout<<"Minimum dist with class : "<<currentInput<<" , "<< minDist << " , "<<currentClass[currentInput]<<" , " <<(int)classImg->data[currentInput]<<std::endl;

        }

        ///make alll means zero

        for(int i = 0 ; i < 6; i ++){
            for(int j = 0 ; j < 5 ; j ++){
                meansArray[i][j] = 0;
            }
        }
        //calculate new means
        for(int numClasses = 0; numClasses < 6; numClasses++){
            double newMeans = 0;
            //check how many claases
            int count = 0;

            for (int input = 0; input < 5; input++) {
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

        /*    for(int classnum = 0; classnum < 6;  classnum++){
                for(int my = 0 ; my < 9; my++){
                    std::cout<<meansArray[classnum][my]<<std::endl;
                }
            }*/
        classImg->name = ("Class img");
        //display_help(*classImg);
        classImg->print_image(fileNameOutput,classImg);

    }



}

void Texture::normalize_feature(double** input, double** output, int imgSize){

    double sum = 0, mean = 0, stdDev = 0;
    for(int numVector = 0; numVector < imgSize; numVector ++){
        for(int currentIndex = 0; currentIndex < 25; currentIndex++){
            sum = sum + input[numVector][currentIndex];
        }
        mean = sum / 25;
        //std::cout<<"Mean : "<<mean<<std::endl;
        for(int i = 0; i < 25; i++){
            stdDev = stdDev + pow(input[numVector][i] - mean, 2);
            stdDev = sqrt(stdDev / 25);
        }

        for(int j = 0 ; j < 25; j++){
            double newValue = (input[numVector][j] - mean) / stdDev;
            output[numVector][j] = newValue;
        }
    }


}


