//
// Created by Manasa Manohara on 2/18/18.
//

#ifndef HW_2_PROB3D_MORPH_H
#define HW_2_PROB3D_MORPH_H

#include <memory.h>
#include "Image.h"
#include <string>

class Morph {
public:
    Morph(Image& image, bool printDebug);
    virtual ~Morph();
    void get_stage1(Image* reducedChannel,Image* outputChannel, int neighborhood);
    void get_nearest_neighbor(Image* reducedChannel, int neighborhood, int row, int col,int* searchPixels);
    static bool matchmasks(int* bondPattern, int* searchPixels,int searchPixelsSize, int neighborhood, int maskSize, int currentBond);
    int calc_bond(int* searchPixels, int searchPixelsSize);
    void get_stage_2(Image* inputChannel, Image* outputChannel,Image* firstStageInput,int neighborhood);
    int get_size_of_stars(Image* inputChannel, int iteration_index, int* countArray, int previousCount);
    void get_stars_count(Image* inputChannel);


private:

    void check_if_hit_stage1(Image* outputChannel,int*searchPixels,int searchPixelsSize,int neighborhood,int currentBond, int currPixelLocation );
    int check_if_hit_stage2(int* searchPixels, int hit);
    const Image* inImg;
    bool printDebug;


    int s_bond1[36] = {0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1};
    int s_bond2[36] = {0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0};
    int s_bond3 [72] = {0,0,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,1,0,0,1};
    int tk_bond4[36] = {0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,1,0};
    int stk_bond4[36] = {0,0,1,0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,1};
    int st_bond5[72] = {1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0, 0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1};
    int st_bond6[90] = {1,1,0,0,1,1,0,0,1,0,1,1,1,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,0,1,1,0,1,1};

    int stk_bond6[72] = {1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,0,1,1,0,1,1};
    int stk_bond7[36] = {1,1,1,0,1,1,0,0,1,1,1,1,1,1,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,1,0,1,1,1,1,1};
    int stk_bond8[36] = {0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,1,1,1,1};
    int stk_bond9[72] = {1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1};
    int stk_bond10[36] = {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1};
    int k_bond11[36] = {1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1};

};



#endif //HW_2_PROB3D_MORPH_H
