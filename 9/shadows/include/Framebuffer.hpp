/** @file Framebuffer.hpp
 *  @brief Creates an offscreen texture to render to.
 *
 * The 'Create' function needs to be called before using the  
 * framebuffer.
 *
 *  @author Mike
 *  @bug No known bugs.
 *
 */
#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include <glad/glad.h>
#include <memory>

// Each Framebuffer can have a custom shader so we
// are forward declaring the class.
class Shader;

class Framebuffer{
public:
    // Default Constructor
    Framebuffer();
    // Destructor
    ~Framebuffer();
    // Create the framebuffer
    void Create(int width, int height);
    // Select our framebuffer
    void Bind();
    // Update our framebuffer once per frame for any
    // changes that may have occurred.
    void Update();
    // Done with our framebuffer
    void Unbind();
    // Draws the screen quad
    void DrawFBO();
private: 
    // Creates a quad that will be overlaid on top of the screen
    void SetupScreenQuad(float x,float y, float w, float h);
// public member variables
public:
    std::shared_ptr<Shader> m_fboShader;
    // Our framebuffer also needs a texture.
    unsigned int m_colorBuffer_id;
// private member variables
private:
    // Framebuffer id
    unsigned int m_fbo_id; 
    // Finally create our render buffer object
    unsigned int m_rbo_id;
    // Store our screen buffer
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;

};

#endif
