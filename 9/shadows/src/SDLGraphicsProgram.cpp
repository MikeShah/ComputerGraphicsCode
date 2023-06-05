#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "Texture.hpp"
#include "ShaderManager.hpp"
#include "MeshMaker.hpp"
#include "FBO.hpp"
// Include the 'Renderer.hpp' which deteremines what
// the graphics API is going to be for OpenGL
#include "Renderer.hpp"
#include "Vertex.hpp"

// The glad library helps setup OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <SDL2/SDL_keycode.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FBO
std::shared_ptr<FBO> g_depthFBO;
// meshes
std::shared_ptr<MeshMaker> g_plane;
std::shared_ptr<MeshMaker> g_cube;
std::shared_ptr<MeshMaker> g_light;
// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
void renderScene(const Shader &shader);

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// The window we'll be rendering to
	m_window = NULL;

    m_width = w;
    m_height = h;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
	}
    //Use OpenGL 4.1 core
	// NOTE: To students, try to revert to MAJOR=3 and MINOR=3 if you have difficulties
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //Create window
    m_window = SDL_CreateWindow( "Lab",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            m_width,
                            m_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    // Check if Window did not create.
    if( m_window == NULL ){
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    //Create an OpenGL Graphics Context
    m_openGLContext = SDL_GL_CreateContext( m_window );
    if( m_openGLContext == NULL){
        std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    // Initialize GLAD Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        std::cerr << "Failed to iniitalize GLAD\n";
        exit(EXIT_FAILURE);
    }

    // If initialization succeeds then print out a list of errors in the constructor.
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL errors detected during initialization\n\n");

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// renders the 3D scene
// --------------------
void renderScene(std::string shadername)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
	ShaderManager::Instance().GetShader(shadername)->SetUniformMatrix4fv("model", &model[0][0]);
	g_plane->RenderMesh();
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    ShaderManager::Instance().GetShader(shadername)->SetUniformMatrix4fv("model", &model[0][0]);
    g_cube->RenderMesh();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    ShaderManager::Instance().GetShader(shadername)->SetUniformMatrix4fv("model", &model[0][0]);
    g_cube->RenderMesh();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    ShaderManager::Instance().GetShader(shadername)->SetUniformMatrix4fv("model", &model[0][0]);
    g_cube->RenderMesh();
}





//Loops forever!
void SDLGraphicsProgram::SetLoopCallback(std::function<void(void)> callback){
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    // Setup shaders for the node.
	ShaderManager::Instance().CreateNewShader("shadowmapping",
											  "./shaders/3.1.3.shadow_mapping.vs",
    										  "./shaders/3.1.3.shadow_mapping.fs");  

	ShaderManager::Instance().CreateNewShader("shadowmappingdepth",
											  "./shaders/3.1.3.shadow_mapping_depth.vs",
											  "./shaders/3.1.3.shadow_mapping_depth.fs");

	ShaderManager::Instance().CreateNewShader("shadowmappingdepthdebug",
											  "./shaders/3.1.3.debug_quad.vs",
											  "./shaders/3.1.3.debug_quad_depth.fs");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    g_plane = std::make_shared<MeshMaker>(VERTEXLAYOUTS::VERTEX3TN);
    g_plane->CreateTexturedPlane(15.0f,15.0f);

    g_cube = std::make_shared<MeshMaker>(VERTEXLAYOUTS::VERTEX3TN);
    g_cube->CreateCube(1.0f,1.0f,1.0f);

    g_light= std::make_shared<MeshMaker>(VERTEXLAYOUTS::VERTEX3TN);
    g_light->CreateCube(0.5f,0.5f,0.5f);

    // load textures
    // -------------
    // Create a texture
    Texture brickTexture("./../../common/textures/brick.ppm");

    // configure depth map FBO
    // -----------------------
    g_depthFBO = std::make_shared<FBO>();
    g_depthFBO->CreateDepthMapFBO(m_width,m_height);


    // shader configuration
    // --------------------
	ShaderManager::Instance().UseShader("shadowmapping");
	ShaderManager::Instance().GetShader("shadowmapping")->SetUniform1i("diffuseTexture", 0);
	ShaderManager::Instance().GetShader("shadowmapping")->SetUniform1i("shadowMap", 1);

	ShaderManager::Instance().UseShader("shadowmappingdepthdebug");
	ShaderManager::Instance().GetShader("shadowmappingdepthdebug")->SetUniform1i("depthMap", 0);

    // lighting info
    // -------------
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);


    // Create a renderer
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(m_width,m_height);    
    // Create our terrain
    std::shared_ptr<Terrain> myTerrain = std::make_shared<Terrain>(512,512,"./../../common/textures/terrain2.ppm");
    myTerrain->LoadTextures("./../../common/textures/colormap.ppm","./../../common/textures/detailmap.ppm");

    // Create a node for our terrain 
    std::shared_ptr<SceneNode> terrainNode;
    terrainNode = std::make_shared<SceneNode>(myTerrain,"./shaders/vert.glsl","./shaders/frag.glsl");
    // Set our SceneTree up
    renderer->setRoot(terrainNode);


    // Set a default position for our camera
    renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.5f,5.0f);
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.5f;

    // Center our mouse
    SDL_WarpMouseInWindow(m_window,m_width/2,m_height/2);

    // Get a pointer to the keyboard state
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // While application is running
    while(!quit){
        // For our terrain setup the identity transform each frame
        // By default set the terrain node to the identity
        // matrix.
//        terrainNode->GetLocalTransform().LoadIdentity();
        // Invoke(i.e. call) the callback function
        callback();

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }
        } // End SDL_PollEvent loop.

        // Move left or right
        if(keyboardState[SDL_SCANCODE_LEFT]){
            renderer->GetCamera(0)->MoveLeft(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_RIGHT]){
            renderer->GetCamera(0)->MoveRight(cameraSpeed);
        }

        // Move forward or back
        if(keyboardState[SDL_SCANCODE_UP]){
            renderer->GetCamera(0)->MoveForward(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_DOWN]){
            renderer->GetCamera(0)->MoveBackward(cameraSpeed);
        }

        // Move up or down
        if(keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT])   {
            renderer->GetCamera(0)->MoveUp(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_LCTRL] || keyboardState[SDL_SCANCODE_RCTRL]){
            renderer->GetCamera(0)->MoveDown(cameraSpeed);
        }
		

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // change light position over time
        static float time = 0.0f;
        lightPos.x = sin(time) * 3.0f;
        lightPos.z = cos(time) * 2.0f;
        lightPos.y = 5.0 + cos(time) * 1.0f;
        time += 0.01;
        if (time > 360){
            time =0.0f;
        }
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
		
		ShaderManager::Instance().UseShader("shadowmappingdepth");
		ShaderManager::Instance().GetShader("shadowmappingdepth")->SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);


        g_depthFBO->Bind();
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            brickTexture.Bind();
            renderScene("shadowmappingdepth");
        g_depthFBO->Unbind();

        // reset viewport
        glViewport(0, 0, m_width, m_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, m_width, m_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ShaderManager::Instance().UseShader("shadowmapping");
        // For now, //45.0f is the FOV
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width/ (float)m_height, 0.1f, 100.0f);
        glm::mat4 view = renderer->GetCamera(0)->GetWorldToViewmatrix();
		ShaderManager::Instance().GetShader("shadowmapping")->SetUniformMatrix4fv("projection", &projection[0][0]);
		ShaderManager::Instance().GetShader("shadowmapping")->SetUniformMatrix4fv("view", &view[0][0]);
        // set light uniforms
		ShaderManager::Instance().GetShader("shadowmapping")->SetUniform3f("viewPos", renderer->GetCamera(0)->GetEyeXPosition(),renderer->GetCamera(0)->GetEyeYPosition(),renderer->GetCamera(0)->GetEyeZPosition());
		ShaderManager::Instance().GetShader("shadowmapping")->SetUniform3f("lightPos", lightPos[0],lightPos[1],lightPos[2]);
		ShaderManager::Instance().GetShader("shadowmapping")->SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        brickTexture.Bind();
        g_depthFBO->BindTexture(1);
		renderScene("shadowmapping");
        // Final render our light, which we do not want in our shadow pass
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPos[0],lightPos[1],lightPos[2]));
        model = glm::scale(model, glm::vec3(0.5f));
        ShaderManager::Instance().GetShader("shadowmapping")->SetUniformMatrix4fv("model", &model[0][0]);
        g_light->RenderMesh();

        // Update our scene through our renderer
        renderer->Update();
        // Render our scene using our selected renderer
        //renderer->Render();
        // render Depth map to quad for visual debugging
        // ---------------------------------------------
		ShaderManager::Instance().UseShader("shadowmappingdepthdebug");
		ShaderManager::Instance().GetShader("shadowmappingdepthdebug")->SetUniform1f("near_plane", near_plane);
		ShaderManager::Instance().GetShader("shadowmappingdepthdebug")->SetUniform1f("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        g_depthFBO->BindTexture(0);


        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();


}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
