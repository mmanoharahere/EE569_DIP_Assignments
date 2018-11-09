//
// Created by Manasa Manohara on 2/9/18.
//

//
// Created by Manasa Manohara on 2/9/18.
//

#include "Image.h"
#include <iostream>

Image::Image(int width, int height, int numChannels) {
    this->width = width;
    this->height = height;
    this->numChannels = numChannels;
    this->data = nullptr;
}

void Image::allocate() {
    if (this->data == nullptr) {
        this->data = new unsigned char[width * height * numChannels];
        memset(this->data, 0, getNumOfBytes()); // Initialize the allocated memory to zero
    }
}

void Image::deallocate() {
    if (this->data != nullptr) {
        delete [] this->data;
        this->data = nullptr; // pointer to a pointer
    }
}

Image::~Image() {
    if (this->data != nullptr) {
        std::cout << "Image class is leaking..." << std::endl;
    }
}

size_t Image::getNumOfBytes() {
    return static_cast<size_t>(this->width * this->height * this->numChannels);
}
void Image::print_image(const std::string& fileName, Image* outputImg){
    FILE* file = nullptr;
    file=fopen(fileName.c_str(),"wb");
    int imgSize = outputImg->width * outputImg->height * outputImg->numChannels;
    if (file == nullptr) {
        std::cout << "Cannot open file: " << fileName << std::endl;
        exit(1);
    }
    fwrite(outputImg->data, sizeof(unsigned char), imgSize, file);
    fclose(file);
}





