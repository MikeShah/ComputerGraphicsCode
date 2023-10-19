/* Compilation on Linux: 
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/ -lSDL2 -ldl
*/
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

// Third Party Libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numbers>


// Our libraries
#include "Camera.hpp"
#include "Light.hpp"
#include "util.hpp"
#include "STL.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.
#include "globals.hpp"
// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

/**
* Initialization of the graphics application. Typically this will involve setting up a window
* and the OpenGL Context (with the appropriate version)
*
* @return void
*/
void InitializeProgram(){
		// Initialize SDL
		if(SDL_Init(SDL_INIT_VIDEO)< 0){
				std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
				exit(1);
		}
	
		// Setup the OpenGL Context
		// Use OpenGL 4.1 core or greater
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Create an application window using OpenGL that supports SDL
		g.gGraphicsApplicationWindow = SDL_CreateWindow( "Lighting",
													SDL_WINDOWPOS_UNDEFINED,
													SDL_WINDOWPOS_UNDEFINED,
													g.gScreenWidth,
													g.gScreenHeight,
													SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( g.gGraphicsApplicationWindow == nullptr ){
				std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
				exit(1);
		}

		// Create an OpenGL Graphics Context
		g.gOpenGLContext = SDL_GL_CreateContext( g.gGraphicsApplicationWindow );
		if( g.gOpenGLContext == nullptr){
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
				exit(1);
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
				std::cout << "glad did not initialize" << std::endl;
				exit(1);
		}

    // Setup Light(s)
	g.gBunny = new STLFile;

    g.gLight.Initialize();
	g.gBunny->Initialize();
}


/**
* PreDraw
* Typically we will use this for setting some sort of 'state'
* Note: some of the calls may take place at different stages (post-processing) of the
* 		 pipeline.
* @return void
*/
void PreDraw(){
	// Disable depth test and face culling.
    glEnable(GL_DEPTH_TEST);                    // NOTE: Need to enable DEPTH Test
		//glDepthFunc(GL_LESS);
    //glDisable(GL_CULL_FACE);

    // Set the polygon fill mode
    glPolygonMode(GL_FRONT_AND_BACK,g.gPolygonMode);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, g.gScreenWidth, g.gScreenHeight);
    glClearColor( 0.0f, 0.53f, 0.66f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


}

/**
* Draw
* The render function gets called once per loop.
* Typically this includes 'glDraw' related calls, and the relevant setup of buffers
* for those calls.
*
* @return void
*/
void Draw(){
	// Draw our Bunny
    g.gBunny->PreDraw();
	g.gBunny->Draw();
    // Draw our light
    g.gLight.PreDraw();
    g.gLight.Draw();
}

/**
* Helper Function to get OpenGL Version Information
*
* @return void
*/
void getOpenGLVersionInfo(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/**
* Function called in the Main application loop to handle user input
*
* @return void
*/
void Input(){
    // Two static variables to hold the mouse position
    static int mouseX=g.gScreenWidth/2;
    static int mouseY=g.gScreenHeight/2; 

	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;
	//Handle events on queue
	while(SDL_PollEvent( &e ) != 0){
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if(e.type == SDL_QUIT){
			std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			g.gQuit = true;
		}
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
			std::cout << "ESC: Goodbye! (Leaving MainApplicationLoop())" << std::endl;
            g.gQuit = true;
        }
        if(e.type==SDL_MOUSEMOTION){
            // Capture the change in the mouse position
            mouseX+=e.motion.xrel;
            mouseY+=e.motion.yrel;
            g.gCamera.MouseLook(mouseX,mouseY);
        }
	}

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        SDL_Delay(250);
        std::cout << "up:" << std::endl;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        SDL_Delay(250); 
        std::cout << "Down:" << std::endl;
    }

    // Camera
    // Update our position of the camera
    if (state[SDL_SCANCODE_W]) {
        g.gCamera.MoveForward(0.1f);
    }
    if (state[SDL_SCANCODE_S]) {
        g.gCamera.MoveBackward(0.1f);
    }
    if (state[SDL_SCANCODE_A]) {
    }
    if (state[SDL_SCANCODE_D]) {
    }

    if (state[SDL_SCANCODE_1]) {
        SDL_Delay(250); // This is hacky in the name of simplicity,
                       // but we just delay the
                       // system by a few milli-seconds to process the 
                       // keyboard input once at a time.
        if(g.gPolygonMode== GL_FILL){
            g.gPolygonMode = GL_LINE;
        }else{
            g.gPolygonMode = GL_FILL;
        }
    }
}

/**
* Main Application Loop
* This is an infinite loop in our graphics application
*
* @return void
*/
void MainLoop(){

    // Little trick to map mouse to center of screen always.
    // Useful for handling 'mouselook'
    // This works because we effectively 're-center' our mouse at the start
    // of every frame prior to detecting any mouse motion.
    SDL_WarpMouseInWindow(g.gGraphicsApplicationWindow,g.gScreenWidth/2,g.gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


	// While application is running
	while(!g.gQuit){
		// Type of start of frame
		Uint32 start = SDL_GetTicks();

		// Handle Input
		Input();
		// Setup anything (i.e. OpenGL State) that needs to take
		// place before draw calls
		PreDraw();
		// Draw Calls in OpenGL
        // When we 'draw' in OpenGL, this activates the graphics pipeline.
        // i.e. when we use glDrawElements or glDrawArrays,
        //      The pipeline that is utilized is whatever 'glUseProgram' is
        //      currently binded.
		Draw();

		// Calculate how much time has elapsed
		// since the start of the frame, and delay
		// for the difference in milliseconds to attempt
		// to get 60 fps for systems that run too fast
		Uint32 elapsedTime = SDL_GetTicks() - start;
		if(elapsedTime < 16){
			SDL_Delay(16-elapsedTime);
		}

		//Update screen of our specified window
		SDL_GL_SwapWindow(g.gGraphicsApplicationWindow);
	}
}

/**
* The last function called in the program
* This functions responsibility is to destroy any global
* objects in which we have create dmemory.
*
* @return void
*/
void CleanUp(){
		//Destroy our SDL2 Window
		SDL_DestroyWindow(g.gGraphicsApplicationWindow );
		g.gGraphicsApplicationWindow = nullptr;

		//Quit SDL subsystems
		SDL_Quit();
}

/**
* The entry point into our C++ programs.
*
* @return program status
*/
int main( int argc, char* args[] ){
    std::cout << "Use w and s keys to move forward and back\n";
    std::cout << "Use mouse to look around\n";
    std::cout << "Use 1 to toggle wireframe\n";
    std::cout << "Press ESC to quit\n";

	// 1. Setup the graphics program
	InitializeProgram();
		
	// 2. Call the main application loop
	MainLoop();	

	// 3. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
