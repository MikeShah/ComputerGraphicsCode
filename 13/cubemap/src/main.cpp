/* Compilation on Linux: 
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/ -lSDL2 -ldl
*/

// Third Party Libraries
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>

// Our libraries
#include "util.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "CubeMapTexture.hpp"
#include "shader.hpp"
#include "MeshFactory.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
int gScreenWidth 						= 640;
int gScreenHeight 						= 480;
SDL_Window* gGraphicsApplicationWindow 	= nullptr;
SDL_GLContext gOpenGLContext			= nullptr;

MeshFactory gPlane;
MeshFactory gSkybox;

// Main loop flag
bool gQuit = false; // If this is quit = 'true' then the program terminates.


struct Resource{
        // We will load a texture here prior
};


Shader  gGraphicsPipelineShaderProgram;
Shader  gCubeMapShaderProgram;

// Shaders
// Here we setup two shaders, a vertex shader and a fragment shader.
// At a minimum, every Modern OpenGL program needs a vertex and a fragment
// shader.
float g_uOffset=-2.0f;
float g_uRotate=0.0f;

// Camera
Camera gCamera;

// Texture
Texture        gTexture;
CubeMapTexture gCubeMapTexture;

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
	gGraphicsApplicationWindow = SDL_CreateWindow( "SkyBox and Abstraction Program",
													SDL_WINDOWPOS_UNDEFINED,
													SDL_WINDOWPOS_UNDEFINED,
													gScreenWidth,
													gScreenHeight,
													SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	// Check if Window did not create.
	if( gGraphicsApplicationWindow == nullptr ){
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Create an OpenGL Graphics Context
	gOpenGLContext = SDL_GL_CreateContext( gGraphicsApplicationWindow );
	if( gOpenGLContext == nullptr){
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Initialize GLAD Library
	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
		std::cout << "glad did not initialize" << std::endl;
		exit(1);
	}
	
    // Setup a textured plane
    gPlane.MakeTexturedPlane();
    gTexture.LoadTexture("./cat3.ppm");
    // Setup a graphics pipeline for basic meshes
	gGraphicsPipelineShaderProgram.CreateShaderProgram("./shaders/vert.glsl","./shaders/frag.glsl");

	// Setup the skybox
    gSkybox.MakeSkyBox();
    std::vector<std::string> filepaths = {"./right.ppm","./left.ppm","./top.ppm","./bottom.ppm","./back.ppm","./front.ppm"};
    gCubeMapTexture.LoadCubeMapTexture(filepaths);
    // Setup a graphics pipeline for skyboxes 
	gCubeMapShaderProgram.CreateShaderProgram("./shaders/cube_vert.glsl","./shaders/cube_frag.glsl");

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
	// Disable depth test and face culling.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor( 1.f, 1.f, 0.f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Use our shader
    gGraphicsPipelineShaderProgram.Bind();

    // Model transformation by translating our object into world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,g_uOffset)); 

    // Update our model matrix by applying a rotation after our translation
//    model = glm::rotate(model,glm::radians(g_uRotate),glm::vec3(0.0f,1.0f,0.0f));

    // Setup MVP matrix
	glDepthMask(GL_FALSE);

	gCubeMapShaderProgram.Bind();
    gCubeMapShaderProgram.SetUniformMatrix4fv("u_ViewMatrix",gCamera.GetViewMatrix());
    gCubeMapShaderProgram.SetUniformMatrix4fv("u_Projection",gCamera.GetPerspectiveMatrix(gScreenWidth,gScreenHeight));

	gCubeMapTexture.Bind();
    gCubeMapShaderProgram.SetUniform1i("u_skybox",0);

	gSkybox.BindAndDraw(false,36);

	glDepthMask(GL_TRUE);

    // Setup MVP matrix
	gGraphicsPipelineShaderProgram.Bind();

    gGraphicsPipelineShaderProgram.SetUniformMatrix4fv("u_ModelMatrix",model);
    gGraphicsPipelineShaderProgram.SetUniformMatrix4fv("u_ViewMatrix",gCamera.GetViewMatrix());
    gGraphicsPipelineShaderProgram.SetUniformMatrix4fv("u_Projection",gCamera.GetPerspectiveMatrix(gScreenWidth,gScreenHeight));
    // Setup texture uniform
    gTexture.Bind();
    gGraphicsPipelineShaderProgram.SetUniform1i("u_DiffuseTexture",0);

    gPlane.BindAndDraw(true,6);



	// Stop using our current graphics pipeline
    gGraphicsPipelineShaderProgram.Unbind();
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
	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;
	//Handle events on queue
	while(SDL_PollEvent( &e ) != 0){
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if(e.type == SDL_QUIT){
			std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			gQuit = true;
		}
	}

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        g_uOffset+=0.01f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        g_uOffset-=0.01f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        g_uRotate-=1.0f;
        std::cout << "g_uRotate: " << g_uRotate<< std::endl;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        g_uRotate+=1.0f;
        std::cout << "g_uRotate: " << g_uRotate<< std::endl;
    }

    // Camera
    // Update our position of the camera
    if (state[SDL_SCANCODE_W]) {
        gCamera.MoveForward(0.1f);
    }
    if (state[SDL_SCANCODE_S]) {
        gCamera.MoveBackward(0.1f);
    }
    if (state[SDL_SCANCODE_A]) {
        gCamera.MoveLeft(0.1f);
    }
    if (state[SDL_SCANCODE_D]) {
        gCamera.MoveRight(0.1f);
    }
    // Update the mouse look of the camera
    // Center the mouse in the window
    int mouseX, mouseY;
    SDL_GetGlobalMouseState(&mouseX,&mouseY);
    gCamera.MouseLook(mouseX,mouseY);
}


/**
* Main Application Loop
* This is an infinite loop in our graphics application
*
* @return void
*/
void MainLoop(){

    // For the mouse look
    // Nice to center mouse in the window
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, 640/2,480/2);

	// While application is running
	while(!gQuit){
		// Handle Input
		Input();
		// Draw Calls in OpenGL
        // When we 'draw' in OpenGL, this activates the graphics pipeline.
        // i.e. when we use glDrawElements or glDrawArrays,
        //      The pipeline that is utilized is whatever 'glUseProgram' is
        //      currently binded.
		Draw();
		//Update screen of our specified window
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
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
    SDL_DestroyWindow(gGraphicsApplicationWindow );
    gGraphicsApplicationWindow = nullptr;



    //Quit SDL subsystems
    SDL_Quit();
}


/**
* The entry point into our C++ programs.
*
* @return program status
*/
int main( int argc, char* args[] ){
    std::cout << "Use arrow keys to move and rotate\n";
    std::cout << "Use wasd to move\n";

    // 1. Setup the graphics program
    InitializeProgram();

    // 4. Call the main application loop
    MainLoop();	

    // 5. Call the cleanup function when our program terminates
    CleanUp();

    return 0;
}
