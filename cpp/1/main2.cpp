// Live code Sept. 6, 2023
// g++ -std=c++20 main2.cpp -o prog && ./prog
// ./prog > output.ppm
// display output.ppm
#include <iostream>
#include <vector>

// Create our own pixel type
struct RGB{
   uint8_t r,g,b; 
};

// Create an image type 
struct Image{
    unsigned int width, height;
    std::vector<RGB> pixels;

    // Constructor
    Image(int w, int h, int wrap){
        width = w;
        height = h;

        if(wrap > 255){
            wrap = 255;
        }

        for(int i=0; i < width; i++){
            for(int j=0; j < height; j++){
                RGB singlepixel;
                singlepixel.r = i % wrap;
                singlepixel.g = j % wrap;

                pixels.push_back(singlepixel);
            }
        }
    }


    // Output a gradient
    void output(){
    // If I don't 'cast' to an 'int', then
    // my value is treated as a 'char'
        std::cout << "P3\n";
        std::cout << (int)width << " " << (int)height << "\n";
        std::cout << (int)255 << "\n";
        for(int i=0; i < width; i++){
            for(int j=0; j < height; j++){
                std::cout << (int)pixels[j*width+i].r << " "
                          << (int)pixels[j*width+i].g << " "
                          << (int)pixels[j*width+i].b << "\n";

            }
        }

    }
};

// Entry point into the program
int main(){

    // Instance of our 'image' class
    Image img(300,300,64);
    // Running a member function on our image class instance.
    img.output();

    return 0;
}
