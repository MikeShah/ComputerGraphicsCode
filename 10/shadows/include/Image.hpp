/** @file Image.hpp
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

class Image {
public:
    // Constructor for creating an image
    Image (std::string filepath);
    // Destructor
    ~Image();
    // Loads a PPM from memory.
    void LoadPPM(bool flip);
    // Return the width
    inline int GetWidth(){
        return m_width;
    }
    // Return the height
    inline int GetHeight(){
        return m_height;
    }
    // Bytes per pixel
    inline int GetBPP(){
        return m_BPP;
    }
    // Set a pixel a particular color in our data
    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    // Display the pixels
    void PrintPixels();
    // Retrieve raw array of pixel data
    uint8_t* GetPixelDataPtr();
    // Returns the red component of a pixel
    inline unsigned int GetPixelR(int x, int y){
        return m_pixelData[(x*3)+m_height*(y*3)];
    }
    // Returns the green component of a pixel
    inline unsigned int GetPixelG(int x, int y){
        return m_pixelData[(x*3)+m_height*(y*3)+1];
    }
    // Returns the blue component of a pixel
    inline unsigned int GetPixelB(int x, int y){
        return m_pixelData[(x*3)+m_height*(y*3)+2];
    }
private:
    // Filepath to the image loaded
    std::string m_filepath;
    // Raw pixel data
    uint8_t* m_pixelData;
    // Size and format of image
    int m_width{0}; // Width of the image
    int m_height{0}; // Height of the image
    int m_BPP{0};   // Bits per pixel (i.e. how colorful are our pixels)
	std::string magicNumber; // magicNumber if any for image format
};

#endif
