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
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <numbers>

// Our libraries
#include "Camera.hpp"
#include "util.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
int gScreenWidth 						= 640;
int gScreenHeight 						= 480;
SDL_Window* gGraphicsApplicationWindow 	= nullptr;
SDL_GLContext gOpenGLContext			= nullptr;


// Main loop flag
bool gQuit = false; // If this is quit = 'true' then the program terminates.

// OpenGL Objects
// Vertex Array Object (VAO)
// Vertex array objects encapsulate all of the items needed to render an object.
// For example, we may have multiple vertex buffer objects (VBO) related to rendering one
// object. The VAO allows us to setup the OpenGL state to render that object using the
// correct layout and correct buffers with one call after being setup.
GLuint gVertexArrayObjectBunny= 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint  gVertexBufferObjectBunny[3];

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram	= 0;

// Camera
Camera gCamera;

// Draw wireframe mode
GLenum gPolygonMode = GL_FILL;

// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

struct Light{
    float mAmbientIntensity{0.5f};
    glm::vec3 mPosition;

	GLuint mShaderID;	
    GLuint mVAO;
    GLuint mVBO;

    /// Constructor
	Light(){}

    // Initialization function that can be called after
    // OpenGL has been setup
    void initialize()
    {
		std::string vertexShaderSource      = LoadShaderAsString("./shaders/light_vert.glsl");
		std::string fragmentShaderSource    = LoadShaderAsString("./shaders/light_frag.glsl");

		mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);

		// Vertice positions don't really matter, because all
		// I really want is a 'point' in space
		const GLfloat vertices[] = {0.0f,0.0f,0.0f};  
		// Vertex Arrays Object (VAO) Setup
		glGenVertexArrays(1, &mVAO);
		// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
		glBindVertexArray(mVAO);

		// Vertex Buffer Object (VBO) creation
		glGenBuffers(1, &mVBO);

		// Populate our vertex buffer objects
		// Position information (x,y,z)
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		// NOTE: vertices is 'stack allocated' so I can use 'sizeof'
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);
		
		// Unbind our currently bound Vertex Array Object
		glBindVertexArray(0);
		// Disable any attributes we opened in our Vertex Attribute Arrray,
		// as we do not want to leave them open. 
		glDisableVertexAttribArray(0);
	}

	void Draw(){
		// Use the light shader prior to drawing
		glUseProgram(mShaderID);

        // Update Light Position in a 'circle' on flat z-axis 
        static float increment=0.0f;
        increment += 0.017f;
        if(increment > 2*M_PI) { increment=0.0f;}
        mPosition.x = cos(increment) * 3;
        mPosition.y = 2.0f;
        mPosition.z = sin(increment) * 3;
        
       glPointSize( 10.0f); 
        // Model transformation by translating our object into world space
        glm::mat4 model = glm::translate(glm::mat4(1.0f),mPosition); 

        // Retrieve our location of our Model Matrix
        GLint u_ModelMatrixLocation = glGetUniformLocation( mShaderID,"u_ModelMatrix");
        if(u_ModelMatrixLocation >=0){
            glUniformMatrix4fv(mShaderID,1,GL_FALSE,&model[0][0]);
        }else{
            std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }

        // Setup Uniform variables here
        // Retrieve our location of our Model Matrix
        GLint u_LightPos = glGetUniformLocation( mShaderID,"u_LightPos");
        std::cout << "x: " << mPosition[0] << std::endl;
        if(u_LightPos >=0){
            glUniform3fv(u_LightPos, 1, &mPosition[0]);
        }else{
            std::cout << "Could not find u_LightPos, maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }

        glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                                 (float)gScreenWidth/(float)gScreenHeight,
                                                 0.1f,
                                                 20.0f);

        // Retrieve our location of our perspective matrix uniform 
        GLint u_ProjectionLocation= glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_Projection");
        if(u_ProjectionLocation>=0){
            glUniformMatrix4fv(u_ProjectionLocation,1,GL_FALSE,&perspective[0][0]);
        }else{
            std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }

		// Enable our attributes
		glBindVertexArray(mVAO);

		//Render data
		glDrawArrays(GL_POINTS,0,3);

		// Stop using our current graphics pipeline
		// Note: This is not necessary if we only have one graphics pipeline.
		glUseProgram(0);
        glPointSize( 1.0f); 
	}

};

// Light object
Light gLight;


/**
* Create the graphics pipeline
*
* @return void
*/
void CreateGraphicsPipeline(){

    std::string vertexShaderSource      = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource    = LoadShaderAsString("./shaders/frag.glsl");

	gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);
}


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
	gGraphicsApplicationWindow = SDL_CreateWindow( "Lighting",
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

    // Setup Lights
    gLight.initialize();
	
}


// The STL format is a simple 'triangle soup' format that outputs
// one normal per triangle, and then the following three triangles
// The .stl bunny was obtained from: https://en.m.wikipedia.org/wiki/File:Stanford_Bunny.stl
// And then modified in blender3D.
class STL{
private:
	std::vector<float> mVertices;
	std::vector<float> mNormals;
	std::vector<float> mColors;
	
public:
	// Load the STL File
	STL(std::string filepath){
		// File input stream for our file
		std::ifstream myFile(filepath.c_str());

		// Check for file existance before proceeding.
		if(myFile.is_open()){
			std::string line;
			while(std::getline(myFile,line)){
				auto vertex_pos = line.find("vertex ");
				auto normal_pos = line.find("facet normal ");

				if(vertex_pos != std::string::npos){ 
					std::string s = line.substr(vertex_pos+7); // "vertex " -- 7 characters including the space
					std::stringstream stream(s);   
					std::string token;
					while(stream >> token){
						mVertices.push_back(std::stof(token.c_str()));
					}
					
				}else if(normal_pos != std::string::npos){
					std::string s = line.substr(normal_pos+13); // "facet normal " -- 13 characters including the space
					std::stringstream stream(s);   
					std::string token;
					while(stream >> token){
						//std::cout << "normal: " << token << std::endl;
						mNormals.push_back(std::stof(token.c_str()));
					}
					// Copy the normals twice
					int threeBack = mNormals.size()-1-3;
					mNormals.push_back(threeBack);
					mNormals.push_back(threeBack+1);
					mNormals.push_back(threeBack+2);
					threeBack = mNormals.size()-1-3;
					mNormals.push_back(threeBack);
					mNormals.push_back(threeBack+1);
					mNormals.push_back(threeBack+2);
				}
			}
		}
	}

	std::vector<float> GetVertices() const{
		std::cout << "mVertices:" << mVertices.size() << std::endl;
		return mVertices;
	}

	std::vector<float> GetNormals() const{
		std::cout << "mNormals :" << mNormals.size() << std::endl;
		return mNormals;
	}

};




/**
* Setup your geometry during the vertex specification step
*
* @return void
*/
/**
* Setup your geometry during the vertex specification step
*
* @return void
*/
void VertexSpecification(){

    // STL Model Format
	STL stl("./bunny_centered.stl");
	std::vector<float> vertices = stl.GetVertices();
	std::vector<float> normals  = stl.GetNormals();

	// Vertex Arrays Object (VAO) Setup
	glGenVertexArrays(1, &gVertexArrayObjectBunny);
	// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
	glBindVertexArray(gVertexArrayObjectBunny);

	// Vertex Buffer Object (VBO) creation
	glGenBuffers(3, gVertexBufferObjectBunny);

	// Populate our vertex buffer objects
    // Position information (x,y,z)
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectBunny[0]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

    // Normals
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectBunny[1]);
	glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,0,(void*)0);
    // Color information (r,g,b)
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectBunny[2]);
	glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

	// Unbind our currently bound Vertex Array Object
	glBindVertexArray(0);
	// Disable any attributes we opened in our Vertex Attribute Arrray,
	// as we do not want to leave them open. 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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
    glDisable(GL_CULL_FACE);

    // Set the polygon fill mode
    glPolygonMode(GL_FRONT_AND_BACK,gPolygonMode);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor( 0.1f, 4.f, 7.f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Use our shader
	glUseProgram(gGraphicsPipelineShaderProgram);

    // Model transformation by translating our object into world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f)); 
    static float rot=0.0f;
//    rot += 0.1f; // Uncomment to add a rotation
    model = glm::rotate(model,glm::radians(rot),glm::vec3(0.0f,1.0f,0.0f)); 

    // Retrieve our location of our Model Matrix
    GLint u_ModelMatrixLocation = glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_ModelMatrix");
    if(u_ModelMatrixLocation >=0){
        glUniformMatrix4fv(u_ModelMatrixLocation,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Update the View Matrix
    GLint u_ViewMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram,"u_ViewMatrix");
    if(u_ViewMatrixLocation>=0){
        glm::mat4 viewMatrix = gCamera.GetViewMatrix();
        glUniformMatrix4fv(u_ViewMatrixLocation,1,GL_FALSE,&viewMatrix[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    // Projection matrix (in perspective) 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)gScreenWidth/(float)gScreenHeight,
                                             0.1f,
                                             20.0f);

    // Retrieve our location of our perspective matrix uniform 
    GLint u_ProjectionLocation= glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_Projection");
    if(u_ProjectionLocation>=0){
        glUniformMatrix4fv(u_ProjectionLocation,1,GL_FALSE,&perspective[0][0]);
    }else{
        std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
    


    // Setup the Lights 
    GLint loc = glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_LightPos");
    if(loc >=0){
        glUniform3fv(loc, 1, &gLight.mPosition[0]);
    }else{
        std::cout << "Could not find u_LightPos" << std::endl;
        exit(EXIT_FAILURE);
    }
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
    // Enable our attributes
	glBindVertexArray(gVertexArrayObjectBunny);

    //Render data
    glDrawArrays(GL_TRIANGLES,0,4404);

	// Stop using our current graphics pipeline
	// Note: This is not necessary if we only have one graphics pipeline.
    glUseProgram(0);

    // Draw our lights
    gLight.Draw();
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
    static int mouseX=gScreenWidth/2;
    static int mouseY=gScreenHeight/2; 

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
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
			std::cout << "ESC: Goodbye! (Leaving MainApplicationLoop())" << std::endl;
            gQuit = true;
        }
        if(e.type==SDL_MOUSEMOTION){
            // Capture the change in the mouse position
            mouseX+=e.motion.xrel;
            mouseY+=e.motion.yrel;
            gCamera.MouseLook(mouseX,mouseY);
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
        gCamera.MoveForward(0.1f);
    }
    if (state[SDL_SCANCODE_S]) {
        gCamera.MoveBackward(0.1f);
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
        if(gPolygonMode== GL_FILL){
            gPolygonMode = GL_LINE;
        }else{
            gPolygonMode = GL_FILL;
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
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow,gScreenWidth/2,gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


	// While application is running
	while(!gQuit){
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

    // Delete our OpenGL Objects
    glDeleteBuffers(3, gVertexBufferObjectBunny);
    glDeleteVertexArrays(1, &gVertexArrayObjectBunny);

	// Delete our Graphics pipeline
    glDeleteProgram(gGraphicsPipelineShaderProgram);

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
	
	// 2. Setup our geometry
	VertexSpecification();
	
	// 3. Create our graphics pipeline
	// 	- At a minimum, this means the vertex and fragment shader
	CreateGraphicsPipeline();
	
	// 4. Call the main application loop
	MainLoop();	

	// 5. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
