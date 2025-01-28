import std.stdio;
import sdl_abstraction;
import opengl_abstraction;
import bindbc.sdl;
import bindbc.opengl;

struct Mesh{
		GLuint mVAO;
		GLuint mVBO;
}

struct Pipeline{
		static GLuint[string] sPipeline;
}

static ~this(){
	// On program termination, dump out all the shader names
	writeln("Shaders | GLuint:name");
	foreach(entry,value ; Pipeline.sPipeline){
		writeln("\t",value,":",entry);
	}
}

// Free function to make it easy to check which graphics pipeline to use
void UsePipeline(string name){
		if(name in Pipeline.sPipeline){
				glUseProgram(Pipeline.sPipeline[name]);
		}else{
				writeln("Error, could not find graphics pipeline object with name: ", name);
		}
}

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
		pragma(msg, mVertexData.length);

		// Vertex Arrays Object (VAO) Setup
		glGenVertexArrays(1, &m.mVAO);
		// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
		glBindVertexArray(m.mVAO);

		// Vertex Buffer Object (VBO) creation
		glGenBuffers(1, &m.mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m.mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexData.length* GLfloat.sizeof, mVertexData.ptr, GL_STATIC_DRAW);

		// Vertex attributes
		// Atribute #0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLfloat.sizeof*6, cast(void*)0);

		// Attribute #1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GLfloat.sizeof*6, cast(GLvoid*)(GLfloat.sizeof*3));

		// Unbind our currently bound Vertex Array Object
		glBindVertexArray(0);
		// Disable any attributes we opened in our Vertex Attribute Arrray,
		// as we do not want to leave them open. 
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		return m;
}

struct GraphicsApp{
		bool mGameIsRunning=true;
		SDL_GLContext mContext;
		SDL_Window* mWindow;

		Mesh mTriangleMesh;

		int mScreenWidth = 640;
		int mScreenHeight = 480;

		/// Setup OpenGL and any libraries
		this(int width, int height){
				mScreenWidth = width;
				mScreenHeight = height;

				// Setup SDL OpenGL Version
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
				// We want to request a double buffer for smooth updating.
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

				// Create an application window using OpenGL that supports SDL
				mWindow = SDL_CreateWindow( "dlang - OpenGL",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								mScreenWidth,
								mScreenHeight,
								SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

				// Create the OpenGL context and associate it with our window
				mContext = SDL_GL_CreateContext(mWindow);

				// Load OpenGL Function calls
				auto retVal = LoadOpenGLLib();


				// Check OpenGL version
				GetOpenGLVersionInfo();
		}

		~this(){
				// Destroy our context
				SDL_GL_DeleteContext(mContext);
				// Destroy our window
				SDL_DestroyWindow(mWindow);
		}

		/// Handle input
		void Input(){
				// Store an SDL Event
				SDL_Event event;
				while(SDL_PollEvent(&event)){
						if(event.type == SDL_QUIT){
								writeln("Exit event triggered (probably clicked 'x' at top of the window)");
								mGameIsRunning= false;
						}
						if(event.type == SDL_KEYDOWN){
								if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
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
				Pipeline.sPipeline["basic"] = BuildBasicShader("./pipelines/basic/basic.vert","./pipelines/basic/basic.frag");
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
				UsePipeline("basic");
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
				// Setup the graphics scene
				SetupScene();
				// Run the graphics application loop
				while(mGameIsRunning){
						AdvanceFrame();
				}
		}
}
