#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif


#include "Framebuffer.hpp"
#include "Shader.hpp"

#include <glad/glad.h>


Framebuffer::Framebuffer(){
    // (1) ======= Setup shader
    m_fboShader = std::make_shared<Shader>();
    // Setup shaders for the Framebuffer Object
    std::string fboVertexShader = m_fboShader->LoadShaderAsString("./shaders/fboVert.glsl");
    std::string fboFragmentShader = m_fboShader->LoadShaderAsString("./shaders/fboFrag.glsl");
    // Actually create our shader
    m_fboShader->CreateShader(fboVertexShader,fboFragmentShader);       
    // (2) ======= Setup quad to draw to
    // Setup the screen quad
    // x and y of 0.0 put the quad in the top left corner
    // w and h of 1.0 stretch quad across entire screen
    SetupScreenQuad(0.0f,0.0f,1.0f,1.0f);
}

// Destructor
Framebuffer::~Framebuffer(){
    glDeleteFramebuffers(1,&m_fbo_id); 
    glDeleteVertexArrays(1,&m_quadVAO);
    glDeleteBuffers(1,&m_quadVBO);
}


// Create the framebuffer
// We create this in a second step, because we need
// width and height information
// TODO: What happens if the window resizes?
//       Answer: Need to regenerate our buffer
void Framebuffer::Create(int width, int height){

    // Generate a framebuffer
    glGenFramebuffers(1, &m_fbo_id);
    // Select the buffer we have just generated
    Bind();
    // Create a color attachment texture
    glGenTextures(1, &m_colorBuffer_id);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_colorBuffer_id,0);
    // Create our render buffer object
    glGenRenderbuffers(1,&m_rbo_id);
    glBindRenderbuffer(GL_RENDERBUFFER,m_rbo_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);
    // Deselect our buffers
    Unbind();
}
// Select our framebuffer
void Framebuffer::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
}

// Update our framebuffer once per frame for any
// changes that may have occurred.
void Framebuffer::Update(){
    // Select our framebuffer
    m_fboShader->Bind();
    // Set the uniforms in our current shader

    // For our object, we apply the texture in the following way
    // Note that we set the value to 0, because we have bound
    // our texture to slot 0.
    m_fboShader->SetUniform1i("u_DiffuseMap",0);  
}

// Done with our framebuffer
void Framebuffer::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

// Draws the screen quad
// This is the actual rendering of our FBO to the screen.
// Typically this would be called after 'update'
void Framebuffer::DrawFBO(){
    glBindVertexArray(m_quadVAO);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer_id);   // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ============== Private Member Functions ==============

// Creates a quad that will be overlaid on top of the screen
// x and y specify the position of the quad.
// w and h specify the width and the height
// TODO: add x1,x2, etc. to draw FBO over a range in the scene.
void Framebuffer::SetupScreenQuad(float x,float y, float w, float h){



    float quad[] = {
        // First triangle
        // x and y coordinates followed by texture coordinates
        x-w, y+h,  0.0f, 1.0f, // x,y,s,t
        x-w, y-h,  0.0f, 0.0f,
        x+w, y-h,  1.0f, 0.0f,
        // Second triangle
        x+-w, y+h,  0.0f, 1.0f,
        x+ w, y-h,  1.0f, 0.0f,
        x+ w, y+h,  1.0f, 1.0f
    };

// screen quad VAO
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

}
