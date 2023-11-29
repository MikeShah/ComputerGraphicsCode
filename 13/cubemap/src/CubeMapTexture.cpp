#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include "CubeMapTexture.hpp"

#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <memory>

// Default Constructor
CubeMapTexture::CubeMapTexture(){

}


// Default Destructor
CubeMapTexture::~CubeMapTexture(){
	// Delete our texture from the GPU
	glDeleteTextures(1,&m_cubeMapTextureID);

    for(int i=m_images.size()-1; i > -1 ; i--){
        if(m_images[i] != nullptr){
            delete m_images[i];
        }
    }
}

void CubeMapTexture::LoadCubeMapTexture(const std::vector<std::string> filepaths){
    // Assign m_filepaths to filepaths passed in for debuggin purposes
    m_filepaths = filepaths;

	// Generate a buffer for our texture
    glGenTextures(1,&m_cubeMapTextureID);
    // Similar to our vertex buffers, we now 'select'
    // a texture we want to bind to.
    // Note the type of data is 'GL_TEXTURE_CUBE_MAP'
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTextureID);
    // Now we are going to setup some information about
    // our cube map texture.
    // GL_TEXTURE_MIN_FILTER - How texture filters (linearly, etc.)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // Wrap mode describes what to do if we go outside the boundaries of
    // texture.
    // Note: For the Cube Map we need the 'R' value 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    for(int i=0; i < filepaths.size(); i++){
        // This method loads .ppm files of pixel data
        Image* temp = new Image;
        temp->LoadPPM(filepaths[i],true);
        m_images.push_back(temp);

        // At this point, we are now ready to load and send some data to OpenGL.
        // Little trick to add +i to the target in order to  just move to the next
        // enum automatically
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
                        0 ,
                        GL_RGB,
                        m_images[i]->GetWidth(),
                        m_images[i]->GetHeight(),
                        0,
                        GL_RGB,
                        GL_UNSIGNED_BYTE,
                        m_images[i]->GetPixelDataPtr()); // Here is the raw pixel data
    }
    // We are done with our texture data so we can unbind.
    // Generate a mipmap
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);                        
	// We are done with our texture data so we can unbind.    
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


// slot tells us which slot we want to bind to.
// We can have multiple slots. By default, we
// will set our slot to 0 if it is not specified.
void CubeMapTexture::Bind(unsigned int slot) const{
	// Using OpenGL 'state' machine we set the active texture
	// slot that we want to occupy. Again, there could
	// be multiple at once.
	// At the time of writing, OpenGL supports 8-32 depending
	// on your hardware.
    glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTextureID);
}

void CubeMapTexture::Unbind(){
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


