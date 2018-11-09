
#ifndef COLORTOGRAY_HW_1_1_IMAGE_H
#define COLORTOGRAY_HW_1_1_IMAGE_H

#include <memory.h>
#include <string>

class Image {
public:
    Image(int width, int height, int numChannels);
    void allocate();
    void deallocate();
    virtual ~Image();
    void print_image(const std::string& fileName,Image* outputImg);

    size_t getNumOfBytes();

    int width;
    int height;
    int numChannels;

    unsigned char* data;
    std::string name;

};


#endif //COLORTOGRAY_HW_1_1_IMAGE_H
