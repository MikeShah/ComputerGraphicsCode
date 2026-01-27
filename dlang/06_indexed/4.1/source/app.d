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
    GLuint mVAO;    // Vertex Array Object
    GLuint mVBO;    // Vertex Buffer Object
    GLuint mIBO;    // Index Buffer
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
    gWindow = SDL_CreateWindow( "dlang indexed mesh example - OpenGL",
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
    /* Contains duplicates
    const GLfloat[] mVertexData =
        [
            // First Triangle
           -0.5f,  -0.5f, 0.0f, 	// Bottom-Left vertex position
            0.5f,  -0.5f, 0.0f,  	// Bottom-right vertex position
            0.5f,   0.5f, 0.0f,  	// Top-Right vertex position
            // Second Triangle
           -0.5f,  -0.5f, 0.0f, 	// Bottom-Left vertex position
            0.5f,   0.5f, 0.0f,  	// Top-Right vertex position
           -0.5f,   0.5f, 0.0f,  	// Top-Left  vertex position
        ];
    */
    const GLfloat[] mVertexData =
        [
           -0.5f,  -0.5f, 0.0f, // Index 0 - Bottom-Left vertex position
            0.5f,  -0.5f, 0.0f, // Index 1 - Bottom-right vertex position
            0.5f,   0.5f, 0.0f, // Index 2 - Top-Right vertex position
           -0.5f,   0.5f, 0.0f, // Index 3 - Top-Left  vertex position
        ];
    //                            first triangle
    //                            v v v
    const GLuint [] mIndexData = [0,1,2, 0,2,3];
    //                                   ^ ^ ^
    //                                   second triangle


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
            GLfloat.sizeof*3, // Stride 
            cast(void*)0);    // Offset


    // Index Buffer Object(IBO) (or Element Array Buffer (EBO))
    glGenBuffers(1,&gMesh.mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,gMesh.mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLuint.sizeof*mIndexData.length,
                 mIndexData.ptr,
                 GL_STATIC_DRAW);


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
    glBindVertexArray(gMesh.mVAO);

    // Call our draw function on the currently bound
    // vertex array (and any associated buffers, e.g. VBO and now EBO)
    // GL_TRIANGLES - tells us to pull out 3 indices at a time
    // '6' here is the number of indices to draw.
    // GL_UNSIGNED_INT is the type of the indice array we originally sent in
    // 'null' is the offset
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,null);

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

    // Run the graphics application loop
    while(gGameIsRunning){
        Input();
        Update();
        Render();
    }
}
