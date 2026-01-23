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
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	// We want to request a double buffer for smooth updating.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create an application window using OpenGL that supports SDL
	gWindow = SDL_CreateWindow( "dlang minimal example - OpenGL",
			640,
			480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

	// Vertex Buffer Object (VBO) creation
	glCreateBuffers(1,&gMesh.mVBO);
	glNamedBufferStorage(gMesh.mVBO, mVertexData.length* GLfloat.sizeof, mVertexData.ptr, 0);

	// Vertex Arrays Object (VAO) Setup
	glCreateVertexArrays(1, &gMesh.mVAO);

	// Associate Vertex Buffer with Vertex Array Object
	// @param1: VAO we are working with (Direct State Access (DSA)
	// @param2: Bind VBO to '0' binding point for this specific VAO -- this is not
	// @param3: the 'VBO' number, because these could repeat or be swapped.
	// @param4: This is the VBO we bind to '0'
	// @param5: Offset into data
	// @param6: Stride of data
	glVertexArrayVertexBuffer(gMesh.mVAO,0,gMesh.mVBO,0,GLfloat.sizeof*3); 
	// Enable our needed Vertex attributes
	glEnableVertexArrayAttrib(gMesh.mVAO,0);
	// Associate VAO attribute
	glVertexArrayAttribBinding(gMesh.mVAO,0,0);

	// Set up each attribute
	// Attribute #1
	glVertexArrayAttribFormat(gMesh.mVAO,0, 3, GL_FLOAT, GL_FALSE, cast(GLvoid*)0);
}

/// Handle input
void Input(){
	// Store an SDL Event
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_EVENT_QUIT){
			writeln("Exit event triggered (probably clicked 'x' at top of the window)");
			gGameIsRunning= false;
		}
	}
}


/// Update gamestate
void Update(){
}

/// Render the graphics scene
void Render(){
	// Set the color of the pixels in all of our color buffers
	// when we clear the screen.
	// Note: glClearDepth, glClearStencil are other functions to clear.
	glClearColor(0.0f,0.6f,0.8f,1.0f);
	// Clear specific buffers (using a bitmask) with the set clear color.
	glClear(GL_COLOR_BUFFER_BIT);

	// Choose our graphics pipeline 
	glUseProgram(gBasicGraphicsPipeline);

	// Select the VAO which tells how to navigate buffer data
	// and which buffers are currently bound.
	// We need to do this prior to our draw call.
	// Note: When we setup the VAO, it 'remembers' which VBO's
	//       we binded to.
	glBindVertexArray(gMesh.mVAO);

	// Call our draw function on the currently bound
	// vertex array (and any associated buffers, e.g. VBO)
	// This 'activates' and starts our graphics pipeline, sending
	// data to our shader in triples (i.e. GL_TRIANGLES)
	glDrawArrays(GL_TRIANGLES,0,3);

	// OpenGL 'draws' into a default backbuffer, and then we
	// 'flip' what is in the backbuffer to the frontbuffer.
	// The 'front' and 'back' buffers make a 'framebuffer', which
	// we'll learn about later.
	SDL_GL_SwapWindow(gWindow);
}

/// Free any resources acuqired
void CleanUp(){
	// Destroy our context
	SDL_GL_DestroyContext(gContext);
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

	// Enable some debugging
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&message_callback, null);

	// Run the graphics application loop
	while(gGameIsRunning){
		Input();
		Update();
		Render();
	}
}

// Note: The 'nothrow' and 'extern(C)' calling convention here is important
extern(C) void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* user_param) nothrow
{
	auto src_str = (GLenum source) {
		switch (source)
		{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			default: assert(0);
		}
	};

	auto type_str = (GLenum type) {
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			default: return "not sure";
		}
	};

	auto severity_str = (GLenum severity) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "not sure";
		}
	};

	auto result_src = src_str(source);
	auto result_type= type_str(type);
	auto result_severity = severity_str(severity);

	// Provide some basic help 
	debug writeln(`=====REPORT FROM DEBUG CALLBACK========`);
	debug writeln(`Note: Sometimes these are errors, sometimes these are just notifications from the driver`);
	debug writeln(`Note: Notifications are often fine behavior for example, and you can filter/ignore them`);
	debug	writeln(result_src,", ",result_type,", ",result_severity,", id=", id, "\n\tmessage: ", cast(string)message[0..length]);
	debug writeln(`=======================================`);
}
