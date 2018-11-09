//
// Created by Manasa Manohara on 2/18/18.
//

#ifndef HW_2_PROB3B_IMAGE_H
#define HW_2_PROB3B_IMAGE_H


#include <memory.h>
#include <string>

class Image {
public:
    Image(int width, int height, int numChannels);
    void allocate();
    void deallocate();
    void print_image(const std::string& fileName, Image* outputImg);
    virtual ~Image();

    size_t getNumOfBytes();

    int width;
    int height;
    int numChannels;

    unsigned char* data;
    std::string name;

};



#endif //HW_2_PROB3B_IMAGE_H
