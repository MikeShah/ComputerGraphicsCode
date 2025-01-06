#include "STL.hpp"

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include <glad/glad.h>
#include "util.hpp"

#include "globals.hpp"


// Load the STL File
STLFile::STLFile(){
    // File input stream for our file
    std::ifstream myFile("bunny_centered.stl");

    // Check for file existance before proceeding.
    if(myFile.is_open()){
        std::string line;
        while(std::getline(myFile,line)){
            auto vertex_pos = line.find("vertex ");
            auto normal_pos = line.find("facet normal ");

            if(vertex_pos != std::string::npos){ 
                std::string s = line.substr(vertex_pos+7); // "vertex " -- 7 characters including the space
                std::stringstream stream(s);   
                std::string token;
                while(stream >> token){
                    mVertices.push_back(std::stof(token.c_str()));
                }
                
            }else if(normal_pos != std::string::npos){
                std::string s = line.substr(normal_pos+13); // "facet normal " -- 13 characters including the space
                std::stringstream stream(s);   
                std::string token;
                while(stream >> token){
                    //std::cout << "normal: " << token << std::endl;
                    mNormals.push_back(std::stof(token.c_str()));
                }
                // Copy the normals twice somce STL files only provide 1 normal
                int threeBack = mNormals.size()-1-3;
                mNormals.push_back(threeBack);
                mNormals.push_back(threeBack+1);
                mNormals.push_back(threeBack+2);
                threeBack = mNormals.size()-1-3;
                mNormals.push_back(threeBack);
                mNormals.push_back(threeBack+1);
                mNormals.push_back(threeBack+2);
            }
        }

		// Compute the surface normal manually -- STL file is not trustworthy...
        // Iterate through 9 vertices at a time
        for(int i=0; i < mVertices.size(); i+=9){
            glm::vec3 v1 (mVertices[i],mVertices[i+1],mVertices[i+2]);
            glm::vec3 v2 (mVertices[i+3],mVertices[i+4],mVertices[i+5]);
            glm::vec3 v3 (mVertices[i+6],mVertices[i+7],mVertices[i+8]);

            glm::vec3 edge1 = v2 - v1;
            glm::vec3 edge2 = v3 - v2;

            glm::vec3 normal = cross(edge1,edge2);
            std::cout << "mine:" << normal.x << "," << normal.y << "," << normal.z << std::endl;
            std::cout << "them:" <<  mNormals[i] << "," << mNormals[i+1] << "," <<  mNormals[i+2] << std::endl;

            // Replace the normlals
            mNormals[i+0] = normal.x;
            mNormals[i+1] = normal.y;
            mNormals[i+2] = normal.z;

            mNormals[i+3] = normal.x;
            mNormals[i+4] = normal.y;
            mNormals[i+5] = normal.z;

            mNormals[i+6] = normal.x;
            mNormals[i+7] = normal.y;
            mNormals[i+8] = normal.z;

        }


    }
}

STLFile::~STLFile(){
    // Delete our OpenGL Objects
    glDeleteBuffers(3, mVBO);
    glDeleteVertexArrays(1, &mVAO);

    // Delete our Graphics pipeline
    glDeleteProgram(mShaderID);
}

void STLFile::Initialize(){
    std::string vertexShaderSource      = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource    = LoadShaderAsString("./shaders/frag.glsl");

		mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);

		// Vertex Arrays Object (VAO) Setup
		glGenVertexArrays(1, &mVAO);
		// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
		glBindVertexArray(mVAO);

		// Vertex Buffer Object (VBO) creation
		glGenBuffers(3, mVBO);

		// Populate our vertex buffer objects
        // Position information (x,y,z)
		glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
		glBufferData(GL_ARRAY_BUFFER,mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

        // Normals
		glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
		glBufferData(GL_ARRAY_BUFFER,mNormals.size() * sizeof(float), mNormals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,0,(void*)0);
        // Color information (r,g,b)
		glBindBuffer(GL_ARRAY_BUFFER, mVBO[2]);
		glBufferData(GL_ARRAY_BUFFER,mNormals.size() * sizeof(float), mNormals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

		// Unbind our currently bound Vertex Array Object
		glBindVertexArray(0);
		// Disable any attributes we opened in our Vertex Attribute Arrray,
		// as we do not want to leave them open. 
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
}

void STLFile::PreDraw(){
    // Use our shader
	glUseProgram(mShaderID);

    // Model transformation by translating our object into world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f)); 
    static float rot=0.0f;
    //      rot += 0.1f; // Uncomment to add a rotation
    model = glm::rotate(model,glm::radians(rot),glm::vec3(0.0f,1.0f,0.0f)); 

    // Retrieve our location of our Model Matrix
    GLint u_ModelMatrixLocation = glGetUniformLocation( mShaderID,"u_ModelMatrix");
    if(u_ModelMatrixLocation >=0){
        glUniformMatrix4fv(u_ModelMatrixLocation,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Update the View Matrix
    GLint u_ViewMatrixLocation = glGetUniformLocation(mShaderID,"u_ViewMatrix");
    if(u_ViewMatrixLocation>=0){
        glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
        glUniformMatrix4fv(u_ViewMatrixLocation,1,GL_FALSE,&viewMatrix[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    // Projection matrix (in perspective) 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)g.gScreenWidth/(float)g.gScreenHeight,
                                             0.1f,
                                             1000.0f);

    // Retrieve our location of our perspective matrix uniform 
    GLint u_ProjectionLocation= glGetUniformLocation( mShaderID,"u_Projection");
    if(u_ProjectionLocation>=0){
        glUniformMatrix4fv(u_ProjectionLocation,1,GL_FALSE,&perspective[0][0]);
    }else{
        std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
    
    // Setup the Lights 
    GLint loc = glGetUniformLocation( mShaderID,"u_LightPos");
    if(loc >=0){
        glUniform3fv(loc, 1, &g.gLight.mPosition[0]);
    }else{
        std::cout << "Could not find u_LightPos" << std::endl;
        exit(EXIT_FAILURE);
    }

}


void STLFile::Draw(){

    //Render data
	glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES,0,4404);
}

std::vector<float> STLFile::GetVertices() const{
    std::cout << "mVertices:" << mVertices.size() << std::endl;
    return mVertices;
}

std::vector<float> STLFile::GetNormals() const{
    std::cout << "mNormals :" << mNormals.size() << std::endl;
    return mNormals;
}


