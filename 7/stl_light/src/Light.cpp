#include "Light.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "globals.hpp"

/// Constructor
Light::Light(){
        mPosition.x = 0.0f;
        mPosition.y = 0.0f;
        mPosition.z = 0.0f;
}

// Initialization function that can be called after
// OpenGL has been setup
void Light::Initialize()
{
    std::string vertexShaderSource      = LoadShaderAsString("./shaders/light_vert.glsl");
    std::string fragmentShaderSource    = LoadShaderAsString("./shaders/light_frag.glsl");

    mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);

    // Draw a cube to represent the light
    const std::vector<GLfloat> vertices{
            -0.1f,-0.1f,-0.1f, // triangle 1 : begin
            -0.1f,-0.1f, 0.1f,
            -0.1f, 0.1f, 0.1f, // triangle 1 : end
            0.1f, 0.1f,-0.1f, // triangle 2 : begin
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f,-0.1f, // triangle 2 : end
            0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f,-0.1f,
            0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            -0.1f,-0.1f, 0.1f,
            0.1f,-0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f,-0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f, 0.1f,-0.1f,
            -0.1f, 0.1f,-0.1f,
            0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f, 0.1f,
            0.1f,-0.1f, 0.1f
    };  

    // Vertex Arrays Object (VAO) Setup
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Vertex Buffer Object (VBO) creation
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    // Setup Vertex Attributes
    // position (x,y,z)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

    // Unbind our currently bound Vertex Array Object
    glBindVertexArray(0);
    // Disable any attributes we opened in our Vertex Attribute Arrray,
    // as we do not want to leave them open. 
    glDisableVertexAttribArray(0);

    std::cout << "Light.mShaderID: " << mShaderID << std::endl;
    std::cout << "Light.mVAO: " << mVAO << std::endl;
    std::cout << "Light.mVBO: " << mVBO << std::endl;
}

void Light::PreDraw(){
    // Use the light shader prior to drawing
    glUseProgram(mShaderID);

    // Update Light position on xz-plane 
    static float increment=0.0f;
    increment += 0.017f;
    if(increment > 2*M_PI) { increment=0.0f;}
    
    mPosition.x = cos(increment) * 2;
    mPosition.y = 0.0f;
    mPosition.z = sin(increment) * 2; 

    // Model transformation by translating our object into world space
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,mPosition); 
    // View Transformation
    glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
    // Projection matrix
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),(float)g.gScreenWidth/(float)g.gScreenHeight, 0.1f,1000.0f);

    // Combine into one matrix to send into GPU
    glm::mat4 mvp = perspective * viewMatrix * model;

    // Retrieve our location of our perspective matrix uniform 
    GLint u_MVP= glGetUniformLocation( mShaderID,"u_MVP");
    if(u_MVP>=0){
        glUniformMatrix4fv(u_MVP,1,GL_FALSE,&mvp[0][0]);
    }else{
        std::cout << "Could not find u_MVP, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

}

void Light::Draw(){

    //Render data
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES,0,36);

    // Stop using our current graphics pipeline
    // Note: This is not necessary if we only have one graphics pipeline.
    glUseProgram(0);
}

