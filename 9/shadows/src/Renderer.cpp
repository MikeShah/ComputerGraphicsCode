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
    // Initialize the root in our scene
    m_root = nullptr;

    // By derfaflt create one framebuffer within the renderere.
    Framebuffer* newFramebuffer = new Framebuffer();
    newFramebuffer->Create(w,h);
    m_framebuffers.push_back(newFramebuffer);
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
    // Delete all of our camera pointers
    for(int i=0; i < m_cameras.size(); i++){
        delete m_cameras[i];
    }
    // Delete all of of our framebuffers
    for(int i=0; i < m_framebuffers.size(); i++){
        delete m_framebuffers[i];
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

    // Setup our uniforms
    // In reality, only need to do this once for this
    // particular fbo because the texture data is 
    // not going to change.
    // NOTE:
    //       Assume for this implementation we only have at most
    //       One framebuffer
    m_framebuffers[0]->Update();
    // Bind to our farmebuffer
    m_framebuffers[0]->Bind();


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
    // TODO: Read this
    // Understand that you should only see a single quad
    // after rendering this, because we are only drawing.
    // one quad
    // This is how we know things are working with our FBO
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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

    // Finish with our framebuffer
    m_framebuffers[0]->Unbind();
    // Now draw a new scene
    // We do not need depth since we are drawing a '2D'
    // image over our screen.
    glDisable(GL_DEPTH_TEST);
    // Clear everything away
    // Clear the screen color, and typically I do this
    // to something 'different' than our original as an
    // indication that I am in a FBO. But you may choose
    // to match the glClearColor
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    // We only have 'color' in our buffer that is stored
    glClear(GL_COLOR_BUFFER_BIT); 
    // Use our new 'simple screen shader'
    m_framebuffers[0]->m_fboShader->Bind();
    // Overlay our 'quad' over the screen
    m_framebuffers[0]->DrawFBO();    
    // Unselect our shader and continue
    m_framebuffers[0]->m_fboShader->Unbind();
}

// Determines what the root is of the renderer, so the
// scene can be drawn.
void Renderer::setRoot(std::shared_ptr<SceneNode> startingNode){
    m_root = startingNode;
}


