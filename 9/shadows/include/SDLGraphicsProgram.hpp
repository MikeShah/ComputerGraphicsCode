/** @file SDLGraphicsProgram.hpp
 *  @brief SDL Class used to setup input and setup of OpenGL.
 *  
 *  This class is used for the initialization of SDL.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

// C++ Libraries
#include <functional>


// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class SDLGraphicsProgram{
public:
    // Constructor
    SDLGraphicsProgram(int w, int h);
    // Destructor
    ~SDLGraphicsProgram();
    // Loop that runs forever
    void SetLoopCallback(std::function<void(void)> callback);
    // Get Pointer to Window
    SDL_Window* GetSDLWindow();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();

private:
	// The Renderer responsible for drawing objects
	// in OpenGL (Or whatever Renderer you choose!)
    // The window we'll be rendering to
    SDL_Window* m_window ;
    // OpenGL context
    SDL_GLContext m_openGLContext;
    // Window width and height
    unsigned int m_width;
    unsigned int m_height;

};

#endif
