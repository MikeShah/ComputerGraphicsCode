// TODO: mouse coordinates are not projected onto texture perfectly, but
//       I wanted to keep this simple enough.
//
/// @file: minimal_compute_shader/app.d
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
GLuint gTextureID;
ComputeShader gComputeShader;
int gMousePressed=0;
float gMouseX;
float gMouseY;

/// Structure for a 'mesh'
struct Mesh{
  GLuint mVAO;
  GLuint mVBO;
}

void CreateImageTexture(ref GLuint tex, int width, int height){
  glGenTextures(1,&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,  GL_FLOAT, null);

  // Specifically bind a texure to an image unit.
  // We need to do this versus glBindTexture
  glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}


struct ComputeShader{
  GLuint mProgramID;

  string shaderCode = 
    `#version 430 core

    // Note: The 'workgroup' size here could very well vary
    //       based on your GPU.
    //       For debugging purposes, you could try x=1,y=1,z=1, and then
    //       optimize from there.
    layout(local_size_x=8,local_size_y=4,local_size_z=1) in;

  // Bind the 'image2D' in our shader to read/write.
  // Effectively we are writing into a texture through the 'image2D'.
  // image2D differs from a texture, because we do not have mipmaps
  layout(rgba32f, binding=0) uniform image2D imgOutput;

  uniform int uMousePressed;
  uniform ivec2 uMouseLocation;

  void main(){
    vec4 value = vec4(1.0, 0.0, 0.0, 1.0);

    // Global index of the current work item
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    value.x = float(texelCoord.x) / (gl_NumWorkGroups.x);
    value.y = float(texelCoord.y) / (gl_NumWorkGroups.y);

    if(uMousePressed==1){
      texelCoord.x = uMouseLocation.x; 
      texelCoord.y = uMouseLocation.y; 
      value = vec4(1.0,1.0,1.0,1.0);
      imageStore(imgOutput, texelCoord, value);
    }

    // Load previous image Data
    // (either from this image, or could be another layer)
    vec4 previous = imageLoad(imgOutput, texelCoord);

    // Update the texture if we have not written 'white' to pixel already.
    if(previous.x != 1.0 && previous.y != 1.0 && previous.z != 1.0){
      imageStore(imgOutput, texelCoord, value);
    }else{
      imageStore(imgOutput, texelCoord, previous);
    }
  }

  `;

  this(const char* computePath){
    GLint success;
    // Compile our compute shader
    uint computeShader;
    computeShader = glCreateShader(GL_COMPUTE_SHADER);
    const char* computeSource = shaderCode.ptr;
    glShaderSource(computeShader, 1, &computeSource, null);
    glCompileShader(computeShader);
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    GLchar[1024] infoLog;
    if(!success){
      glGetShaderInfoLog(computeShader, 1024, null, infoLog.ptr);
      writeln("Compute shader compilation error:", infoLog);
    }
    // Create a program
    mProgramID = glCreateProgram();
    glAttachShader(mProgramID,computeShader);
    glLinkProgram(mProgramID);
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(mProgramID, 1024, null, infoLog.ptr);
      writeln("Compute shader link error:", infoLog);
    }

  }

  // 'Execute' takes care of executing the shader
  void Execute(int state, int x, int y){
    glUseProgram(mProgramID);

    glUniform1i(glGetUniformLocation(mProgramID,"uMousePressed"),state);
    glUniform2i(glGetUniformLocation(mProgramID,"uMouseLocation"),x,480-y);

    glDispatchCompute(640,480,1);
    // Ensure we finish writing before a read
    // GL_ALL_BARRIER_BITS could also be used here if you're not sure,
    // what types of barrier to create at least for testing purposes.
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
}

/// Setup triangle with OpenGL buffers
void VertexSpecification(){

  // Geometry Data
  const GLfloat[] mVertexData =
    [
    -0.75f,  -0.75f, 0.0f, 	// Bottom left vertex position
    0.0,0.0,
    0.75f,  -0.75f, 0.0f,  	// bottom-right vertex position
    1.0,0.0,
    -0.75f,  0.75f, 0.0f,  	// Top-left vertex position
    0.0,1.0,

    0.75f,  -0.75f, 0.0f,  	// bottom-right vertex position
    1.0,0.0,
    0.75f,   0.75f, 0.0f, 	// top-right vertex position
    1.0,1.0,
    -0.75f,  0.75f, 0.0f,  	// Top-left vertex position
    0.0,1.0,
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
      GLfloat.sizeof*5, // Stride 
      cast(void*)0);    // Offset

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 			    // Attribute Number
      2, 			    // Size
      GL_FLOAT, 	    // Data Type	
      GL_FALSE,		    // Normalized? 
      GLfloat.sizeof*5, // Stride 
      cast(void*)(GLfloat.sizeof*3));    // Offset

  // Unbind our currently bound Vertex Array Object
  glBindVertexArray(0);
  // Disable any attributes we opened in our Vertex Attribute Arrray,
  // as we do not want to leave them open. 
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
/// Create a basic shader
/// The result is a 'GLuint' representing the compiled 'program object' or otherwise 'graphics pipeline'
/// that is compiled and ready to execute on the GPU.
GLuint BuildBasicShader(string vertexShaderSourceFilename, string fragmentShaderSourceFilename){

  import std.file, std.string;
  GLuint programObjectID;

  GLchar[1024] infoLog;
  GLint success;
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
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertexShader, 1024, null, infoLog.ptr);
    writeln("Vertex Shader compilation error:", infoLog);
  }

  // Compile fragment shader
  fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
  const char* fSource = fragmentSource.ptr;
  glShaderSource(fragmentShader, 1, &fSource, null);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragmentShader, 1024, null, infoLog.ptr);
    writeln("Fragment Shader compilation error:", infoLog);
  }

  // Create shader pipeline
  programObjectID = glCreateProgram();

  // Link our two shader programs together.
  // Consider this the equivalent of taking two .cpp files, and linking them into
  // one executable file.
  glAttachShader(programObjectID,vertexShader);
  glAttachShader(programObjectID,fragmentShader);
  glLinkProgram(programObjectID);
  glGetProgramiv(programObjectID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(programObjectID, 1024, null, infoLog.ptr);
    writeln("Shader link error:", infoLog);
  }

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


/// Setup SDL and OpenGL Libraries
void InitializeScene(){

  // Setup SDL OpenGL Version
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  // We want to request a double buffer for smooth updating.
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Create an application window using OpenGL that supports SDL
  gWindow = SDL_CreateWindow( "dlang minimal compute shader - OpenGL",
      640,
      480,
      SDL_WINDOW_OPENGL);

  // Create the OpenGL context and associate it with our window
  gContext = SDL_GL_CreateContext(gWindow);

  // Load OpenGL Function calls
  auto retVal = LoadOpenGLLib();

  // Check OpenGL version
  GetOpenGLVersionInfo();


  CreateImageTexture(gTextureID,640,480);
  gComputeShader = ComputeShader("TODO Specify a path");
  gBasicGraphicsPipeline = BuildBasicShader("./pipelines/basic/basic.vert","./pipelines/basic/basic.frag");

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

    // Check for a mouse press
    gMousePressed=0;
    SDL_MouseButtonFlags flags = SDL_GetMouseState(&gMouseX,&gMouseY);
    if(flags==SDL_BUTTON_MASK(SDL_BUTTON_LEFT)){
      gMousePressed=1;
    }
      writeln("mouse pressed:", gMousePressed);
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

  gComputeShader.Execute(gMousePressed,cast(int)gMouseX,cast(int)gMouseY);

  // Choose our graphics pipeline 
  glUseProgram(gBasicGraphicsPipeline);

  GLint location = glGetUniformLocation(gBasicGraphicsPipeline,"tex");
  // When we find the location, modify the uniform variable.
  if(location > -1){
    glUniform1i(location,0);
  }else{
    writeln("Can't find uniform");
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gTextureID);

  // Select the VAO which tells how to navigate buffer data
  // and which buffers are currently bound.
  glBindVertexArray(gMesh.mVAO);

  // Call our draw function on the currently bound
  // vertex array (and any associated buffers, e.g. VBO)
  // This 'activates' and starts our graphics pipeline, sending
  // data to our shader in triples (i.e. GL_TRIANGLES)
  glDrawArrays(GL_TRIANGLES,0,6);

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
