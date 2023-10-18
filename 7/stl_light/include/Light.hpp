#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include "util.hpp"

struct Light{
    float mAmbientIntensity{0.5f};
    glm::vec3 mPosition;

	GLuint mShaderID;	
    GLuint mVAO;
    GLuint mVBO;

    /// Constructor
	Light();

    // Initialization function that can be called after
    // OpenGL has been setup
    void Initialize();

	// Draw a light
	void Draw();

};


#endif
