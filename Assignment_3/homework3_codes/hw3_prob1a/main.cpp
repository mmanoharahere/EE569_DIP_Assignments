
#include "Image.h"
#include "Texture.h"
using namespace std;

#include <iostream>
#include <algorithm>



// Image is not a const because I want to write into this object
bool load_image_from_file(const string& inFileName, Image& img) {

    bool filePresent = false;

    FILE* file = nullptr;
    file = fopen(inFileName.c_str(), "rb");

    // Check if file is not null AND img's data is not null as well
    if (file != nullptr && img.data != nullptr)
    {
        fread(img.data, sizeof(unsigned char), img.getNumOfBytes(), file);
        fclose(file);
        filePresent = true;
    }
    else {
        if (file == nullptr) {
            cout << "File is not open" << endl;
        }
        else if (img.data == nullptr) {
            cout << "Img data is null" << endl;
        }
        else {
            cout << "NULLLL?" << endl;
        }
    }

    return filePresent;
}


/////////////////////// main //////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc != 16)	{	cout << "Syntax Error - Incorrect parameter usage" << endl;
        cout << "Make sure the parameters passed are in the following format:" << endl;
        cout << "<program_name> <input_image1>.raw <input_image2>.raw <input_image3>.raw <input_image4>.raw <input_image5>.raw <input_image6>.raw  <input_image7>.raw  <input_image8>.raw  <input_image9>.raw  <input_image10>.raw  <input_image11>.raw <input_image12>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
        return 0;
    }

    /* initialization of variables */
    int inSizeWidth = atoi(argv[13]); /* number of rows in the image */
    int inSizeHeight = atoi(argv[14]); /* number of columns in the image */
    int inNumChannels = atoi(argv[15]); /* bytes per pixel (RGB = 3, GRAY = 1) */
    // img constructed given dimensions
    // img allocate
    // img load from file
    // do something with img
    // img deallocate

    //do the same for 12 images

    Image *img1 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img1->allocate();
    bool img1Open = load_image_from_file(argv[1], *img1);

    Image *img2 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img2->allocate();
    bool img2Open = load_image_from_file(argv[2], *img2);

    Image *img3 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img3->allocate();
    bool img3Open = load_image_from_file(argv[3], *img3);

    Image *img4 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img4->allocate();
    bool img4Open = load_image_from_file(argv[4], *img4);

    Image *img5 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img5->allocate();
    bool img5Open = load_image_from_file(argv[5], *img5);

    Image *img6 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img6->allocate();
    bool img6Open = load_image_from_file(argv[6], *img6);

    Image *img7 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img7->allocate();
    bool img7Open = load_image_from_file(argv[7], *img7);

    Image *img8 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img8->allocate();
    bool img8Open = load_image_from_file(argv[8], *img8);

    Image *img9 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img9->allocate();
    bool img9Open = load_image_from_file(argv[9], *img9);

    Image *img10 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img10->allocate();
    bool img10Open = load_image_from_file(argv[10], *img10);

    Image *img11 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img11->allocate();
    bool img11Open = load_image_from_file(argv[11], *img11);

    Image *img12 = new Image(inSizeWidth, inSizeHeight, inNumChannels);
    img12->allocate();
    bool img12Open = load_image_from_file(argv[12], *img12);

    bool imgOpen = img1Open && img2Open && img3Open && img4Open && img5Open && img6Open &&img7Open && img8Open && img9Open && img10Open && img11Open && img12Open;

    if (imgOpen) {
        //initialize means array
        int classWidth = 9;
        int classHeight = 4;

        double** meansArrayAllocated = new double* [classHeight];
        for(int i = 0; i < classHeight; i++){
            meansArrayAllocated[i] = new double[classWidth];
            memset(meansArrayAllocated[i], 0, classWidth * sizeof(double));
        }
        double** meansArray = meansArrayAllocated;

        Texture* texture = new Texture(*img1, false);


        int featureWidth = 9;
        int featureHeight = 12;

        double** featureArrayAllocated = new double* [featureHeight];
        for(int i = 0; i < featureHeight; i++){
            featureArrayAllocated[i] = new double[featureWidth];
            memset(featureArrayAllocated[i], 0, featureWidth * sizeof(double));
        }
        double** featureArray = featureArrayAllocated;


        // initialize arrays for every image - energy output
        double img1_energy[9];
        memset(img1_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img1, img1_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[0][j] = img1_energy[j];

        }



        double img2_energy[9];
        memset(img2_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img2, img2_energy);


        for(int j = 0 ; j < featureWidth; j++){
            featureArray[1][j] = img2_energy[j];

        }


        double img3_energy[9];
        memset(img3_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img3, img3_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[2][j] = img3_energy[j];

        }


        double img4_energy[9];
        memset(img4_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img4, img4_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[3][j] = img4_energy[j];
        }


        double img5_energy[9];
        memset(img5_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img5, img5_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[4][j] = img5_energy[j];
        }

        double img6_energy[9];
        memset(img6_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img6 ,img6_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[5][j] = img6_energy[j];

        }

        double img7_energy[9];
        memset(img7_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img7, img7_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[6][j] = img7_energy[j];
        }


        double img8_energy[9];
        memset(img8_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img8, img8_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[7][j] = img8_energy[j];
        }


        double img9_energy[9];
        memset(img9_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img9, img9_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[8][j] = img9_energy[j];
        }


        double img10_energy[9];
        memset(img10_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img10, img10_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[9][j] = img10_energy[j];
        }

        double img11_energy[9];
        memset(img11_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img11, img11_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[10][j] = img11_energy[j];
        }

        double img12_energy[9];
        memset(img12_energy, 0, 9 * sizeof(double));
        texture->texture_analysis(img12, img12_energy);
        for(int j = 0 ; j < featureWidth; j++){
            featureArray[11][j] = img12_energy[j];
        }

// print misclassified texture value to check why it is being misclassified



        //set the initial mean vectors to be one for every prototype of the feature vectors
        for(int j = 0; j < 9; j ++){
            meansArray[0][j] = featureArray[0][j];
            meansArray[1][j] = featureArray[8][j];
            meansArray[2][j] = featureArray[10][j];
            meansArray[3][j] = featureArray[6][j];
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout<<" Lengend for classification : "<<std::endl;
        std::cout<<" 0 : Rock "<<std::endl;
        std::cout<<" 1 : Grass "<<std::endl;
        std::cout<<" 2 : Weave "<<std::endl;
        std::cout<<" 3 : Sand "<<std::endl;
        texture->kmeans(featureArray, 4, meansArray);

    }
    else {

        std::cout << "Image is not open!!" << endl;
    }

    img1->deallocate();
    img2->deallocate();
    img3->deallocate();
    img4->deallocate();
    img5->deallocate();
    img6->deallocate();
    img7->deallocate();
    img8->deallocate();
    img9->deallocate();
    img10->deallocate();
    img11->deallocate();
    img12->deallocate();







}

