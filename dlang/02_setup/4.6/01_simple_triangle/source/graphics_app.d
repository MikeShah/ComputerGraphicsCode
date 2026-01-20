import std.stdio, std.conv;
import sdl_abstraction;
import opengl_abstraction;
import bindbc.sdl;
import bindbc.opengl;

/// Create a basic shader
/// The result is a 'GLuint' representing the compiled 'program object' or otherwise 'graphics pipeline'
/// that is compiled and ready to execute on the GPU.
GLuint BuildBasicShader(string vertexShaderSourceFilename, string fragmentShaderSourceFilename){

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
				}
		}

		import std.file;
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
		const char* vertSource = vertexSource.ptr;
		glShaderSource(vertexShader, 1, &vertSource, null);
		glCompileShader(vertexShader);
		CheckShaderError(vertexShader);

		// Compile fragment shader
		fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragSource = fragmentSource.ptr;
		glShaderSource(fragmentShader, 1, &fragSource, null);
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


struct Mesh{
		GLuint mVAO;
		GLuint mVBO;
}

/// Setup triangle with OpenGL buffers
Mesh MakeTriangleFactory(){
		Mesh m;

		// Geometry Data
		const GLfloat[] mVertexData =
				[
				-0.5f,  -0.5f, 0.0f, 	// Left vertex position
				1.0f,   0.0f, 0.0f, 	// color
				0.5f,  -0.5f, 0.0f,  	// right vertex position
				0.0f,   1.0f, 0.0f,  	// color
				0.0f,   0.5f, 0.0f,  	// Top vertex position
				0.0f,   0.0f, 1.0f,  	// color
				];
		pragma(msg, "Vertex data length: "~mVertexData.length.to!string);

		// Vertex Buffer Object (VBO) creation
		glCreateBuffers(1,&m.mVBO);
		glNamedBufferStorage(m.mVBO, mVertexData.length* GLfloat.sizeof, mVertexData.ptr, 0);

		// Vertex Arrays Object (VAO) Setup
		glCreateVertexArrays(1, &m.mVAO);
		// Associate Vertex Buffer with Vertex Array Object
		glVertexArrayVertexBuffer(	m.mVAO, // VAO we are working with (Direct State Access (DSA)
									0,      // Bind VBO to '0' binding point for this specific VAO -- this is not
										// the 'VBO' number, because these could repeat or be swapped.
									m.mVBO, // This is the VBO we bind to '0'
									0,      // Offset into data
									GLfloat.sizeof*6); // Stride of data

		// Enable our needed Vertex attributes
		glEnableVertexArrayAttrib(m.mVAO,0);
		glEnableVertexArrayAttrib(m.mVAO,1);
		// Associate VAO attribute
		glVertexArrayAttribBinding(m.mVAO,0,0);
		glVertexArrayAttribBinding(m.mVAO,1,0);

		// Set up each attribute
		// Attribute #1
		glVertexArrayAttribFormat(m.mVAO,0, 3, GL_FLOAT, GL_FALSE, cast(GLvoid*)0);
		glVertexArrayAttribFormat(m.mVAO,1, 3, GL_FLOAT, GL_FALSE, cast(GLvoid*)(GLfloat.sizeof*3));


		writeln("Mesh is", m);

		return m;
}


struct GraphicsApp{
		bool mGameIsRunning=true;
		SDL_GLContext mContext;
		SDL_Window* mWindow;

		Mesh mTriangleMesh;
		GLuint mBasicGraphicsPipeline;

		int mScreenWidth = 640;
		int mScreenHeight = 480;

		/// Setup OpenGL and any libraries
		this(int width, int height){
				mScreenWidth = width;
				mScreenHeight = height;

				// Setup SDL OpenGL Version
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
				// We want to request a double buffer for smooth updating.
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

				// Create an application window using OpenGL that supports SDL
				mWindow = SDL_CreateWindow( "dlang - OpenGL",
								mScreenWidth,
								mScreenHeight,
								SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

				// Create the OpenGL context and associate it with our window
				mContext = SDL_GL_CreateContext(mWindow);

				// Load OpenGL Function calls
				auto retVal = LoadOpenGLLib();

				// Check OpenGL version
				GetOpenGLVersionInfo();
		}

		~this(){
				// Destroy our context
				SDL_GL_DestroyContext(mContext);
				// Destroy our window
				SDL_DestroyWindow(mWindow);
		}

		/// Handle input
		void Input(){
				// Store an SDL Event
				SDL_Event event;
				while(SDL_PollEvent(&event)){
						if(event.type == SDL_EVENT_QUIT){
								writeln("Exit event triggered (probably clicked 'x' at top of the window)");
								mGameIsRunning= false;
						}
						if(event.type == SDL_EVENT_KEY_DOWN){
								if(event.key.key == SDLK_ESCAPE){
										writeln("Pressed escape key and now exiting...");
										mGameIsRunning= false;
								}else{
										writeln("Pressed a key ");
								}
						}
				}
		}

		void SetupScene(){
				// Build a basic shader
				mBasicGraphicsPipeline = BuildBasicShader("./pipelines/basic/basic.vert","./pipelines/basic/basic.frag");
				// Build a triangle
				mTriangleMesh = MakeTriangleFactory();
		}

		/// Update gamestate
		void Update(){
		}

		void Render(){
				// Clear the renderer each time we render
				glClearColor(0.0f,0.6f,0.8f,1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// Do opengl drawing
				glUseProgram(mBasicGraphicsPipeline);
				glBindVertexArray(mTriangleMesh.mVAO);
				glDrawArrays(GL_TRIANGLES,0,3);

				// Final step is to present what we have copied into
				// video memory
				SDL_GL_SwapWindow(mWindow);
		}

		/// Process 1 frame
		void AdvanceFrame(){
				Input();
				Update();
				Render();
		}

		/// Main application loop
		void Loop(){
        // Enable error capture
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(&message_callback, null);
				// Setup the graphics scene
				SetupScene();
				// Run the graphics application loop
				while(mGameIsRunning){
						AdvanceFrame();
				}
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
      default: assert(0);
		}
	};

	auto severity_str = (GLenum severity) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
      default: assert(0);
		}
	};

  // Provide some basic help 
  debug	writeln(src_str,", ",type_str,", ",severity_str,", ", id, ": ", cast(string)message[0..length]);
}
