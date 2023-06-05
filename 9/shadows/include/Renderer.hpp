#ifndef RENDERER_HPP
#define RENDERER_HPP
/** @file Renderer.hpp
 *  @brief Renderer is responsible for drawing.
 *
 * 	Renderer is responsible for drawing everything. It
 *	contains a scenegraph node and a camera. We could 
 *	possibly have multiple renderers (if we had multiple
 *	windows for example).
 *
 *	Each renderer thus has it's own camera.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

// The glad library helps setup OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <glad/glad.h>

#include <vector>
#include <memory>

// Forward declarations

#include "SceneNode.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"


class Renderer{
public:
    // The constructor	
    // Sets the width and height of the rendererer draws to
    Renderer(unsigned int w, unsigned int h);
    // Destructor
    ~Renderer();
    // Update the scene
    void Update();
    // Render the scene
    void Render();
    // Sets the root of our renderer to some node to
    // draw an entire scene graph
    void setRoot(std::shared_ptr<SceneNode> startingNode);
    // Returns the camera at an index
    Camera*& GetCamera(unsigned int index){
        if(index > m_cameras.size()-1){
            // TODO: Some sort of error reporting here.
        }
        return m_cameras[index];
    }

// TODO: maybe write getter/setter methods
protected:
    // Root scene node
    std::shared_ptr<SceneNode> m_root;
    // One or more cameras camera per Renderer
    std::vector<Camera*> m_cameras;
    // Store the projection matrix for our camera.
    glm::mat4 m_projectionMatrix;
    // A renderer can have any number of framebuffers
    std::vector<Framebuffer*> m_framebuffers;

private:
    // Screen dimension constants
    int m_screenWidth;
    int m_screenHeight;
};

#endif
