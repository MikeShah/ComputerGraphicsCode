/// @file: 01_simple_triangle/app.d
import std.stdio;
import std.math;
import sdl_abstraction;
import opengl_abstraction;
import bindbc.sdl;
import bindbc.opengl;
import core.stdc.stdlib;

/// Global variables (Bad! But only for demonstration purpose)
Mesh gMesh;
Light gLight;
GLuint gBasicGraphicsPipeline;
bool gGameIsRunning=true;
SDL_GLContext gContext;
SDL_Window* gWindow;

/// Structure for a 'mesh'
struct Mesh{
		GLuint mTexID;	// Texture ID
    GLuint mVAO;
    GLuint mVBO;
}

// A light represented on the CPU side
struct Light{
	float[3] mColor=[0.3,1.0,1.0];
	float[3] mPosition=[0.1,0.1,0.1];
	float 	 mAmbientIntensity=0.0f;
	float 	 mSpecularIntensity=0.5f;
	float 	 mSpecularExponent=32.0f;
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
        // Local nested function -- not meant for otherwise calling freely
        void CheckShaderError(GLuint shaderObject){
            // Retrieve the result of our compilation
            int result;
            // Our goal with glGetShaderiv is to retrieve the compilation status
            glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

            if(result == GL_FALSE){
                int length;
                glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
                GLchar[] errorMessages = new GLchar[length];
                glGetShaderInfoLog(shaderObject, length, &length, errorMessages.ptr);
                writeln(errorMessages);
            }
        }

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
        CheckShaderError(vertexShader);

    // Compile fragment shader
    fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = fragmentSource.ptr;
    glShaderSource(fragmentShader, 1, &fSource, null);
    glCompileShader(fragmentShader);
        CheckShaderError(fragmentShader);

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
				0.0f, 0.0f,						  // vertex texture(vt) coordinate
				0.0f, 0.0f, 1.0f,			// normal
        0.5f,  -0.5f, 0.0f,  	// right vertex position
				1.0f, 0.0f,						  // vertex texture(vt) coordinate
				0.0f, 0.0f, 1.0f,			// normal
        0.0f,   0.5f, 0.0f,  	// Top vertex position
				0.5f, 1.0f,						  // vertex texture(vt) coordinate
				0.0f, 0.0f, 1.0f,			// normal
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
    glVertexAttribPointer(0, 			    // Attribute Number
            3, 			    // Size
            GL_FLOAT, 	    // Data Type	
            GL_FALSE,		    // Normalized? 
            GLfloat.sizeof*8, // Stride 
            cast(void*)0);    // Offset
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 			    // Attribute Number
            2, 			    // Size
            GL_FLOAT, 	    // Data Type	
            GL_FALSE,		    // Normalized? 
            GLfloat.sizeof*8, // Stride 
            cast(void*)(GLfloat.sizeof*3));    // Offset
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 			    // Attribute Number
            3, 			    // Size
            GL_FLOAT, 	    // Data Type	
            GL_FALSE,		    // Normalized? 
            GLfloat.sizeof*8, // Stride 
            cast(void*)(GLfloat.sizeof*5));    // Offset

    // Unbind our currently bound Vertex Array Object
    glBindVertexArray(0);
    // Disable any attributes we opened in our Vertex Attribute Arrray,
    // as we do not want to leave them open. 
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

// Simple PPM image loader
ubyte[] LoadPPMImage(string filename){
	import std.file;
	import std.conv;
	import std.algorithm;
	import std.range;
	ubyte[] result;
	auto f = File(filename);
	int counter=0;
	foreach(line ; f.byLine()){
		counter++;
		if(counter >= 5){
			result ~= line.to!ubyte;
		}
	}
// Flip the image pixels from image space to screen space
	result = result.reverse;
// Swizzle the bytes back to RGB order	
	foreach(rgb ; result.slide(3)){
		rgb.reverse;
	}

	return result;
}

/// Create a new texture
void CreateTexture(string filename, int width, int height){
	// Generate a new texture
	glGenTextures(1,&gMesh.mTexID);
	// Select (i.e. bind) to our newly created texture
	glBindTexture(GL_TEXTURE_2D, gMesh.mTexID);

	ubyte[] image_data = LoadPPMImage(filename);

	glTexImage2D(
		GL_TEXTURE_2D, 	 // 2D Texture
		0,							 // mimap level 0
		GL_RGB, 				 // Internal format for OpenGL
		width,					 // width of incoming data
		height,					 // height of incoming data
		0,							 // border (must be 0)
		GL_RGB,					 // image format
		GL_UNSIGNED_BYTE,// image data 
		image_data.ptr); // pixel array on CPU data

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);	
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


void SetupLights(){
		GLint field1 = glGetUniformLocation(gBasicGraphicsPipeline,"uLight1.mColor");
		GLint field2 = glGetUniformLocation(gBasicGraphicsPipeline,"uLight1.mPosition");
		GLint field3 = glGetUniformLocation(gBasicGraphicsPipeline,"uLight1.mAmbientIntensity");
		GLint field4 = glGetUniformLocation(gBasicGraphicsPipeline,"uLight1.mSpecularIntensity");
		GLint field5 = glGetUniformLocation(gBasicGraphicsPipeline,"uLight1.mSpecularExponent");
		foreach(value ; [field1,field2,field3,field4,field5]){
			if(value < 0){
				writeln("Failed to find: ",value);
			}else{
				writeln("Light Uniform Location(s): ",value);
			}
		}

//		gLight.mAmbientIntensity += .002f;
//		if(gLight.mAmbientIntensity > 2.0f){
//				gLight.mAmbientIntensity = 0.0f;;
//		}
		gLight.mAmbientIntensity = 0.5f;

		// Postion light to move in a circle
		static float inc = 0.0f;
		float radius = 5.0f;
		inc+=0.01;
		gLight.mPosition = [radius*cos(inc),0.0f,radius*sin(inc)];

		glUniform1fv(field1,3,gLight.mColor.ptr);
		glUniform1fv(field2,3,gLight.mPosition.ptr);
		glUniform1f (field3,gLight.mAmbientIntensity);
		glUniform1f (field4,gLight.mSpecularIntensity);
		glUniform1f (field5,gLight.mSpecularExponent);
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

		// Setup our lights
		SetupLights();

		// Select and activate our texture in our shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,gMesh.mTexID);
		GLint sampler1 = glGetUniformLocation(gBasicGraphicsPipeline,"sampler1");
		if(sampler1 <0 ){
				writeln("Could not find 'sampler1'");
				exit(1);
		}else{
			writeln("sampler1 uniform location:",sampler1);
		}
		glUniform1i(sampler1,0);


    // Select the VAO which tells how to navigate buffer data
    // and which buffers are currently bound.
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
		CreateTexture("sample.ppm",256,256);

    // Run the graphics application loop
    while(gGameIsRunning){
        Input();
        Update();
        Render();
    }
}
