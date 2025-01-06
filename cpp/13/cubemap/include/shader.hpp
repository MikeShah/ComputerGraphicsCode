#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <iostream>

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

struct Shader{

    GLuint mProgramObject;

	/// TODO: Come back later and output these names for debugging
	std::string mVertexShader;
	std::string mFragmentShader;

    ~Shader(){
      // Delete our Graphics pipeline
      glDeleteProgram(mProgramObject);
    }
    /**
    * LoadShaderAsString takes a filepath as an argument and will read line by line a file and return a string that is meant to be compiled at runtime for a vertex, fragment, geometry, tesselation, or compute shader.
    * e.g.
    *       LoadShaderAsString("./shaders/filepath");
    *
    * @param filename Path to the shader file
    * @return Entire file stored as a single string 
    */
    std::string LoadShaderAsString(const std::string& filename){
        // Resulting shader program loaded as a single string
        std::string result = "";

        std::string line = "";
        std::ifstream myFile(filename.c_str());

        if(myFile.is_open()){
            while(std::getline(myFile, line)){
                result += line + '\n';
            }
            myFile.close();

        }

        return result;
    }


    /**
    * CompileShader will compile any valid vertex, fragment, geometry, tesselation, or compute shader.
    * e.g.
    *	    Compile a vertex shader: 	CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    *       Compile a fragment shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    *
    * @param type We use the 'type' field to determine which shader we are going to compile.
    * @param source : The shader source code.
    * @return id of the shaderObject
    */
    GLuint CompileShader(GLuint type, const std::string& source){
        // Compile our shaders
        GLuint shaderObject;

        // Based on the type passed in, we create a shader object specifically for that
        // type.
        if(type == GL_VERTEX_SHADER){
            shaderObject = glCreateShader(GL_VERTEX_SHADER);
        }else if(type == GL_FRAGMENT_SHADER){
            shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        }

        const char* src = source.c_str();
        // The source of our shader
        glShaderSource(shaderObject, 1, &src, nullptr);
        // Now compile our shader
        glCompileShader(shaderObject);

        // Retrieve the result of our compilation
        int result;
        // Our goal with glGetShaderiv is to retrieve the compilation status
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

        if(result == GL_FALSE){
            int length;
            glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
            char* errorMessages = new char[length]; // Could also use alloca here.
            glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

            if(type == GL_VERTEX_SHADER){
                std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
            }else if(type == GL_FRAGMENT_SHADER){
                std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
            }
			
			std::cout << "vvv Error somewhere in this shader vvv" << std::endl;
			std::cout << source << std::endl;
			std::cout << "^^^ Error somewhere in this shader ^^^" << std::endl;


            // Reclaim our memory
            delete[] errorMessages;

            // Delete our broken shader
            glDeleteShader(shaderObject);

            return 0;
        }

      return shaderObject;
    }
    /**
    * Creates a graphics program object (i.e. graphics pipeline) with a Vertex Shader and a Fragment Shader
    *
    * @param vertexShaderSource Vertex source code as a string
    * @param fragmentShaderSource Fragment shader source code as a string
    * @return id of the program Object
    */
    GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath){
        std::string vertexShaderSource      = LoadShaderAsString(vertexShaderFilePath);
        std::string fragmentShaderSource    = LoadShaderAsString(fragmentShaderFilePath);

        // Create a new program object
        mProgramObject = glCreateProgram();

        // Compile our shaders
        GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        // Link our two shader programs together.
        // Consider this the equivalent of taking two .cpp files, and linking them into
        // one executable file.
        glAttachShader(mProgramObject,myVertexShader);
        glAttachShader(mProgramObject,myFragmentShader);
        glLinkProgram(mProgramObject);

        // Validate our program
        glValidateProgram(mProgramObject);

        // Once our final program Object has been created, we can
        // detach and then delete our individual shaders.
        glDetachShader(mProgramObject,myVertexShader);
        glDetachShader(mProgramObject,myFragmentShader);
        // Delete the individual shaders once we are done
        glDeleteShader(myVertexShader);
        glDeleteShader(myFragmentShader);

        return mProgramObject;
    }


    void SetUniformMatrix4fv(const GLchar* name, glm::mat4 matrix){  
        // Retrieve our location of our Model Matrix
        GLint matrixLocation = glGetUniformLocation( mProgramObject,name);
        if(matrixLocation >=0){
            glUniformMatrix4fv(matrixLocation,1,GL_FALSE,&matrix[0][0]);
        }else{
            std::cout << "Could not find '" << name << "', maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }
    }

    void SetUniform1i(const GLchar* name, GLint matrix){  
        GLint u_Location = glGetUniformLocation(mProgramObject,name);
        if(u_Location>=0){
            // Setup the slot for the texture
            glUniform1i(u_Location,0);
        }else{
            std::cout << "Could not find '" << name << "', maybe a misspelling?" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void Bind(){
   	    glUseProgram(mProgramObject);
    }
    void Unbind(){
   	    glUseProgram(0);
    }
};


#endif
