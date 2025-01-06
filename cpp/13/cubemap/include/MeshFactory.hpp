#ifndef MESH_FACTORY_HPP
#define MESH_FACTORY_HPP

// Third Party Libraries
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>

struct MeshFactory{

    GLuint  mVertexArrayObject					= 0;
    GLuint 	mVertexBufferObject					= 0;
    GLuint 	mIndexBufferObject                  = 0;

    // Destroy OpenGL buffers
    ~MeshFactory(){
        // Delete our OpenGL Objects
        glDeleteBuffers(1, &mVertexBufferObject);
        glDeleteBuffers(1, &mIndexBufferObject);
        glDeleteVertexArrays(1, &mVertexArrayObject);
    }

    void BindAndDraw(bool indexed, GLsizei count){
        // Enable our attributes
        glBindVertexArray(mVertexArrayObject);
		
		if(indexed){
			//Render data
			glDrawElements(GL_TRIANGLES,count,GL_UNSIGNED_INT,0);
		}else{
			glDrawArrays(GL_TRIANGLES,0,count);
		}
    }

    /**
    * Setup your geometry during the vertex specification step
    *
    * @return void
    */
    void MakeTexturedPlane(){
        // Geometry Data
        const std::vector<GLfloat> vertexData
        {
            // 0 - Vertex
            -0.5f, -0.5f, 0.0f, 	// Left vertex position
            1.0f,  0.0f, 0.0f, 	  // color
             0.0f, 0.0f,       		// texture coordinate
            // 1 - Vertex
            0.5f, -0.5f, 0.0f,  	// right vertex position
            0.0f,  1.0f, 0.0f,  	// color
            1.0f , 0.0f,		      // texture coordinate
            // 2 - Vertex
            -0.5f,  0.5f, 0.0f,  	// Top left vertex position
            0.0f,  0.0f, 1.0f,  	// color
            0.0f, 1.0f, 					// texture coordinate
            // 3 - Vertex
            0.5f,  0.5f, 0.0f,  	// Top-right position
            0.0f,  0.0f, 1.0f,  	// color
            1.0f,1.0f, 			    // texture coordinate
        };


        // Vertex Arrays Object (VAO) Setup
        glGenVertexArrays(1, &mVertexArrayObject);
        // We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
        glBindVertexArray(mVertexArrayObject);

        // Vertex Buffer Object (VBO) creation
        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GL_FLOAT),  vertexData.data(), GL_STATIC_DRAW);					  
     
        // Index buffer data for a quad
        const std::vector<GLuint> indexBufferData {2,0,1, 3,2,1};

        // Setup the Index Buffer Object (IBO i.e. EBO)
        glGenBuffers(1,&mIndexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);

        // Populate our Index Buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexBufferData.size()*sizeof(GLuint),indexBufferData.data(),GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*8,  (void*)0 );

        // Color information
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*8,(GLvoid*)(sizeof(GL_FLOAT)*3) );

        // Now linking up the attributes in our VAO
        // Texture :information
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*8,(GLvoid*)(sizeof(GL_FLOAT)*6)  );

        // Unbind our currently bound Vertex Array Object
        glBindVertexArray(0);
        // Disable any attributes we opened in our Vertex Attribute Arrray,
        // as we do not want to leave them open. 
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void MakeSkyBox(){
        // Geometry Data
        const std::vector<GLfloat> vertexData{
            // positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
        };


        // Vertex Arrays Object (VAO) Setup
        glGenVertexArrays(1, &mVertexArrayObject);
        // We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
        glBindVertexArray(mVertexArrayObject);

        // Vertex Buffer Object (VBO) creation
        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GL_FLOAT), vertexData.data(), GL_STATIC_DRAW);						

        // Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3,  (void*)0 );

        // Unbind our currently bound Vertex Array Object
        glBindVertexArray(0);
        // Disable any attributes we opened in our Vertex Attribute Arrray,
        // as we do not want to leave them open. 
        glDisableVertexAttribArray(0);
    }
};


#endif
