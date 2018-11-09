//
// Created by Manasa Manohara on 3/14/18.
//

#ifndef HW3_PROB2A_IMAGE_H
#define HW3_PROB2A_IMAGE_H
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
#endif //HW3_PROB2A_IMAGE_H
