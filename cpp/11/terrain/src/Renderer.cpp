#include "Renderer.hpp"


// Sets the height and width of our renderer
Renderer::Renderer(unsigned int w, unsigned int h){
    m_screenWidth = w;
    m_screenHeight = h;

    // By default create one camera per render
    // TODO: You could abstract out further functions to create
    //       a camera as a scene node and attach them at various levels.
    Camera* defaultCamera = new Camera();
    // Add our single camera
    m_cameras.push_back(defaultCamera);

    m_root = nullptr;
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
    // Delete all of our camera pointers
    for(int i=0; i < m_cameras.size(); i++){
        delete m_cameras[i];
    }
}

void Renderer::Update(){
    // Here we apply the projection matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane.
    // Note I cannot see anything closer than 0.1f units from the screen.
    m_projectionMatrix = glm::perspective(glm::radians(45.0f),((float)m_screenWidth)/((float)m_screenHeight),0.1f,512.0f);

    // Perform the update
    if(m_root!=nullptr){
        // TODO: By default, we will only have one camera
        //       You may otherwise not want to hardcode
        //       a value of '0' here.
        m_root->Update(m_projectionMatrix, m_cameras[0]);
    }
}

// Initialize clear color
// Setup our OpenGL State machine
// Then render the scene
void Renderer::Render(){

    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.01f, 0.01f, 0.01f, 1.f );
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Nice way to debug your scene in wireframe!
    // Test to see if the 'w' key is pressed for a quick view to toggle
    // the wireframe view.
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_W ] )
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    
    // Now we render our objects from our scenegraph
    if(m_root!=nullptr){
        m_root->Draw();
    }
}

// Determines what the root is of the renderer, so the
// scene can be drawn.
void Renderer::setRoot(SceneNode* startingNode){
    m_root = startingNode;
}


