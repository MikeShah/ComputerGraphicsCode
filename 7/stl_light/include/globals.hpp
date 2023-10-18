#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include "STL.hpp"
#include "Camera.hpp"
#include "Light.hpp"

// Forward Declaration
struct STLFile;

struct Global{
		// Screen Dimensions
		int gScreenWidth 						= 640;
		int gScreenHeight 						= 480;
		SDL_Window* gGraphicsApplicationWindow 	= nullptr;
		SDL_GLContext gOpenGLContext			= nullptr;

		// Main loop flag
		bool gQuit = false; // If this is quit = 'true' then the program terminates.

		// Camera
		Camera gCamera;

		// Draw wireframe mode
		GLenum gPolygonMode = GL_FILL;

		// Light object
		Light gLight;

		// 3D object -- a bunny for the purpose of this demo
		STLFile* gBunny;
};

// External linkage such that the
// global variable is available
// everywhere.
extern Global g;

#endif
