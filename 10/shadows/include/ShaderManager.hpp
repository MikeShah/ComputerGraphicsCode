/** @file ShaderManager.hpp
 *  @brief This Singleton class manages all of the shaders that have been created
 *
 *  The shader manager handles all of the shaders that have been loaded.
 *  
 *
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <unordered_map>
#include <memory>
#include <iostream> // remove this later on

#include "Shader.hpp"

class ShaderManager{
public:
	static ShaderManager& Instance(){
		static ShaderManager* instance = new ShaderManager();
		return *instance;
	}

	// Create a new shader using a vertex Shader and a Fragment shader
	void CreateNewShader(std::string name, 
						std::string vertexShaderFilePath, 
						std::string fragmentShaderFilePath){
		// create a new shader to add to our shader manager
		std::shared_ptr<Shader> s = std::make_shared<Shader>();
		std::string vertexShaderProgramString 	= s->LoadShaderAsString(vertexShaderFilePath);
		std::string fragmentShaderProgramString = s->LoadShaderAsString(fragmentShaderFilePath);
		// Create the shader from the vertex and fragment shader
		// strings that have been loaded
		s->CreateShader( vertexShaderProgramString,
						fragmentShaderProgramString);

		// Now add our shader to a 'map' structure so that we can later look 
		// up the shader program by its name.
		m_shaders[name] = s;
	}

	// Retrieve an existing shader object
	std::shared_ptr<Shader> GetShader(std::string name){
			if(m_shaders.contains(name)){
				return m_shaders[name];
			}else{
				std::cout << "Error, unable to GetShader: " << name << std::endl;
				// TODO: Either throw an exception, or perhaps we can
				// 		 be friendly in a debug build and print out all of the
				// 		 shaders that exist.
				return nullptr;
			}
	}

	// Looks up a previously created shader to use.
	void UseShader(std::string name){
			if(m_shaders.contains(name)){
					m_shaders[name]->Bind();
			}else{
				std::cout << "Error, unable to Use Shader: " << name << std::endl;
				// TODO: Either throw an exception, or perhaps we can
				// 		 be friendly in a debug build and print out all of the
				// 		 shaders that exist.
			}
	}


      
private:
    // ShaderManager Constructor
    ShaderManager() {}
    // ShaderManager Destructor
    ~ShaderManager() {}

	// Holds all of the shader programs
	std::unordered_map<std::string,std::shared_ptr<Shader>> m_shaders;
	// Convenience variable to store the active shader.
	static std::string s_activeShaderName;
};

#endif
