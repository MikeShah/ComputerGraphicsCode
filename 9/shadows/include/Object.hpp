/** @file Object.hpp
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>

// Forward declarations
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Load a texture
    void LoadTexture(std::string fileName);
    // Create a textured quad
    void MakeTexturedQuad(std::string fileName);
    void MakeTexturedQuad2(std::string fileName);
    // How to draw the object
    virtual void Render();
	// Helper method for when we are ready to draw or update our object
	void Bind();
protected: // Classes that inherit from Object are intended to be overridden.

    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one diffuse map
    Texture m_textureDiffuse;
    // Terrains are often 'multitextured' and have multiple textures.
    Texture m_detailMap; // NOTE: Note yet supported
    // Store the objects Geometry
	Geometry m_geometry;
};

#endif
