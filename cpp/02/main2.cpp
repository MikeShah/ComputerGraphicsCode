#include <iostream>
#include <vector>

void setValue(int& alias){
    std::cout << "&alias: "
              << &(alias) << std::endl;
    alias= 9999;
}

struct Pixel{
    uint8_t r,g,b;
};

int main(){

    int x = 42;
    int* pointer  = &x;
    int* pointer2 = &x;

    // I am loading an image that the user chooses.
//    uint8_t* pixels = new uint8_t[100 * 100* sizeof(Pixel)];
//    pixels[0] = 255; // first red value
//    pixels[1] = 255; // first green value
                     //
    std::vector<uint8_t> pixels;
    pixels.push_back(255); // pixels[0] // first red value
    pixels.push_back(253); // pixels[1] // first red value
    uint8_t* data = pixels.data();
    std::cout << "data[1]: "<< static_cast<int>(data[1]) << std::endl;
                     // ...
    // I need to delete the memory that I allocate.
    // Anything I allocate with 'new', I need to 'delete[]'
//    delete[] pixels;

//    pointer = nullptr; // I point to nothing.

    std::cout << "pointer value:" << pointer << std::endl;
    std::cout << "*pointer     :" << *pointer << std::endl;

    std::cout << "&x           :" << &(x) << std::endl;
    setValue(x);

    std::cout << "x is: " << x << std::endl;

    return 0;
}
