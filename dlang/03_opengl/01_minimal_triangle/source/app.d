/// @file: 01_simple_triangle/app.d
import std.stdio;
import sdl_abstraction;
import opengl_abstraction;
import bindbc.sdl;
import bindbc.opengl;

/// Global variables (Bad! But only for demonstration purpose)
Mesh gMesh;
GLuint gBasicGraphicsPipeline;
bool gGameIsRunning=true;
SDL_GLContext gContext;
SDL_Window* gWindow;

/// Structure for a 'mesh'
struct Mesh{
	GLuint mVAO;
	GLuint mVBO;
}

/// Setup SDL and OpenGL Libraries
void InitializeScene(){

		// Setup SDL OpenGL Version
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Create an application window using OpenGL that supports SDL
		gWindow = SDL_CreateWindow( "dlang minimal example - OpenGL",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						640,
						480,
						SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Create the OpenGL context and associate it with our window
		gContext = SDL_GL_CreateContext(gWindow);

		// Load OpenGL Function calls
		auto retVal = LoadOpenGLLib();

		// Check OpenGL version
		GetOpenGLVersionInfo();
		// Build a basic shader
		gBasicGraphicsPipeline = BuildBasicShader("./pipelines/basic/basic.vert","./pipelines/basic/basic.frag");
}

/// Create a basic shader
/// The result is a 'GLuint' representing the compiled 'program object' or otherwise 'graphics pipeline'
/// that is compiled and ready to execute on the GPU.
GLuint BuildBasicShader(string vertexShaderSourceFilename, string fragmentShaderSourceFilename){

		import std.file, std.string;
		GLuint programObjectID;

		// Compile our shaders
		GLuint vertexShader;
		GLuint fragmentShader;

		// Use a string mixin to simply 'load' the text from a file into these
		// strings that will otherwise be processed.
		string vertexSource 	= readText(vertexShaderSourceFilename);
		string fragmentSource 	= readText(fragmentShaderSourceFilename);

		// Compile vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vSource = vertexSource.ptr;
		glShaderSource(vertexShader, 1, &vSource, null);
		glCompileShader(vertexShader);

		// Compile fragment shader
		fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
		const char* fSource = fragmentSource.ptr;
		glShaderSource(fragmentShader, 1, &fSource, null);
		glCompileShader(fragmentShader);

		// Create shader pipeline
		programObjectID = glCreateProgram();

		// Link our two shader programs together.
		// Consider this the equivalent of taking two .cpp files, and linking them into
		// one executable file.
		glAttachShader(programObjectID,vertexShader);
		glAttachShader(programObjectID,fragmentShader);
		glLinkProgram(programObjectID);

		// Validate our program
		glValidateProgram(programObjectID);

		// Once our final program Object has been created, we can
		// detach and then delete our individual shaders.
		glDetachShader(programObjectID,vertexShader);
		glDetachShader(programObjectID,fragmentShader);
		// Delete the individual shaders once we are done
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return programObjectID;
}


/// Setup triangle with OpenGL buffers
void VertexSpecification(){

		// Geometry Data
		const GLfloat[] mVertexData =
				[
				-0.5f,  -0.5f, 0.0f, 	// Left vertex position
				0.5f,  -0.5f, 0.0f,  	// right vertex position
				0.0f,   0.5f, 0.0f,  	// Top vertex position
				];

		// Vertex Arrays Object (VAO) Setup
		glGenVertexArrays(1, &gMesh.mVAO);
		// We bind (i.e. select) to the Vertex Array Object (VAO) 
		// that we want to work withn.
		glBindVertexArray(gMesh.mVAO);

		// Vertex Buffer Object (VBO) creation
		glGenBuffers(1, &gMesh.mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, gMesh.mVBO);
		glBufferData(GL_ARRAY_BUFFER, 
								 mVertexData.length* GLfloat.sizeof, 
								 mVertexData.ptr, 
								 GL_STATIC_DRAW);

		// Vertex attributes
		// Atribute #0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 							  // Attribute Number
													3, 							  // Size
													GL_FLOAT, 				// Data Type	
													GL_FALSE,					// Normalized? 
													GLfloat.sizeof*3, // Stride 
													cast(void*)0); 		// Offset

		// Unbind our currently bound Vertex Array Object
		glBindVertexArray(0);
		// Disable any attributes we opened in our Vertex Attribute Arrray,
		// as we do not want to leave them open. 
		glDisableVertexAttribArray(0);
}

/// Handle input
void Input(){
		// Store an SDL Event
		SDL_Event event;
		while(SDL_PollEvent(&event)){
				if(event.type == SDL_QUIT){
						writeln("Exit event triggered (probably clicked 'x' at top of the window)");
						gGameIsRunning= false;
				}
		}
}


/// Update gamestate
void Update(){
}

/// Render the game scene
void Render(){
		// Clear the renderer each time we render
		glClearColor(0.0f,0.6f,0.8f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Do opengl drawing
		glUseProgram(gBasicGraphicsPipeline);
		glBindVertexArray(gMesh.mVAO);
		glDrawArrays(GL_TRIANGLES,0,3);

		// Final step is to present what we have copied into
		// video memory
		SDL_GL_SwapWindow(gWindow);
}

/// Free any resources acuqired
void CleanUp(){
		// Destroy our context
		SDL_GL_DeleteContext(gContext);
		// Destroy our window
		SDL_DestroyWindow(gWindow);
}
/// Program entry point 
/// NOTE: When debugging, this is '_Dmain'
void main(string[] args)
{

		// Setup the graphics scene
		InitializeScene();
		VertexSpecification();

		// Run the graphics application loop
		while(gGameIsRunning){
				Input();
				Update();
				Render();
		}
}
