/** @file CubeMapTexture.hpp
 *  @brief Create a special cube map for skyboxes and other effects
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef CUBE_MAP_TEXTURE_HPP
#define CUBE_MAP_TEXTURE_HPP

#include "Image.hpp"

#include <glad/glad.h>
#include <string>
#include <vector>

class CubeMapTexture{
public:
    // Constructor
    CubeMapTexture();
    // Destructor
    ~CubeMapTexture();
	// Loads and sets up an actual texture
    void LoadCubeMapTexture(const std::vector<std::string> filepaths);
	// slot tells us which slot we want to bind to.
    // We can have multiple slots. By default, we
    // will set our slot to 0 if it is not specified.
    void Bind(unsigned int slot=0) const;
    // Be done with our texture
    void Unbind();
private:
    // Store a unique ID for the texture
    GLuint m_cubeMapTextureID;
	// Filepath to the image loaded
    std::vector<std::string> m_filepaths;
    // Store whatever image data inside of our texture class.
    std::vector<Image*> m_images;
};

#endif

