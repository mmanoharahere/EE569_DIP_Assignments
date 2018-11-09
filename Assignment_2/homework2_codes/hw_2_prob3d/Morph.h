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
    void invert_image(Image* inputImg, Image* outputImg);
    void get_erosion(Image* reducedChannel,Image* outputChannel,Image* erodedImage,Image* dilatedImage, int neighborhood, int squareElement);
    void get_nearest_neighbor(Image* reducedChannel, int neighborhood, int row, int col,int* searchPixels);
   static bool matchmasks(int* bondPattern, int* searchPixels,int searchPixelsSize, int neighborhood, int maskSize, int currentBond);
    int calc_bond(int* searchPixels, int searchPixelsSize);
    int eroded_image(int squareElement, int* searchPixels);
    void get_dilation(Image* erodedImage, Image* dilatedImage,int dilation );
    int dilated_image(int squareElement, int* searchPixels);
   void  get_stage1(Image* reducedChannel, Image* outputChannel, int neighborhood);
    void get_stage_2(Image* inputChannel, Image* outputChannel,Image* firstStageInput,int neighborhood);
    void get_connected_components(Image* inputImg,  int* connectedComp,Image* outputImg,Image* labelImg, Image* XorImg);
    void get_2Darray(Image* inputArray, int** outputArray, int size);
    void get_2Darray_correct(Image* inputArray, int** outputArray, int size);
    void get_1Darray(Image* outputImg, int** inputArray, int size);
    void get_nearest_labels(Image* reducedChannel, int neighborhood, int row, int col, int* searchPixels, int* XorMask, Image* outPutImg);
    void check_hole_protrusion(int** holeProtArray, int neighborhood, int row, int col ,int** holeValues, int** protValues,int current);
    void rotate_jigsaw(int** holeArray, int** protArray, int numJigsaws);
    //  void unionCoords(int y, int x, int y2, int x2, int imgWidth, int imgHeight, int** inputImg, int* parent);
    //void doUnion(int a, int b, int* parent);


private:

    void check_if_hit_stage1(Image* outputChannel,int*searchPixels,int searchPixelsSize,int neighborhood,int currentBond, int currPixelLocation );
    int check_if_hit_stage2(int* searchPixels, int hit);
    void numberIslands(int** labelAns,int** input_4_2DArray);
    void merge(int row, int col, int count, int** inputArray, int** labelAns);
    int get_size_jigsaw(int **labelAns, int numJigsaws, int* minColIndex, int* maxColIndex,int* minRowIndex, int* maxRowIndex);
    void get_holes_protrusions(Image* labelImg, Image* outPutImg, int numJigsaws, int* minColIndex, int* minRowIndex, int jigsawSize);
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
