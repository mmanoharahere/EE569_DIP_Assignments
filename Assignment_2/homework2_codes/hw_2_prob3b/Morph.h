//
// Created by Manasa Manohara on 2/18/18.
//

#ifndef HW_2_PROB3B_MORPH_H
#define HW_2_PROB3B_MORPH_H

#include <memory.h>
#include "Image.h"
#include <string>

class Morph {
public:
    Morph(Image& image, bool printDebug);
    virtual ~Morph();
    void invert_image(Image* inputImg, Image* outputImg);
    void get_neighbors(Image* reducedChannel,Image* outputChannel, int neighborhood);
    bool multiplymasks(int* bondPattern, int* searchPixels,int searchPixelsSize, int neighborhood, int maskSize, int currentBond);
    int calc_bond(int* searchPixels, int searchPixelsSize, int* searchPixelsAddress);
    void get_stage_2(Image* inputChannel, Image* outputChannel,Image* firstStageInput, int neighborhood);


private:


    // void reduceLayer(const int* searchPixelsAddress, int* searchPixels, int searchPixelsSize, Image* outputChannel,bool isHit);
    //void copyPixels(int* searchPixelsAddress, int* searchPixels,int searchPixelsSize,Image* outputChannel);
    const Image* inImg;
    bool printDebug;


    int t_bond4[72] = {0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,1,0, 0,0,1,0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,1};
    int t_bond5[72] = {1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0, 0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1};

    int t_bond6[90] = {1,1,0,0,1,1,0,0,1,0,1,1,1,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,0,1,1,0,1,1};

    int t_bond7[36] = {1,1,1,0,1,1,0,0,1,1,1,1,1,1,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,1,0,1,1,1,1,1};
    int t_bond8[36] = {0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,1,1,1,1};
    int t_bond9[72] = {1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1};
    int t_bond10[36] = {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1};


};



#endif //HW_2_PROB3B_MORPH_H
