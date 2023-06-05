#ifndef SCENENODE_HPP
#define SCENENODE_HPP
/** @file SceneNode.hpp
 *  @brief SceneNode helps organize a large 3D graphics scene.
 *  
 *  SceneNode helps organize a large 3D graphics scene.
 *  The traversal of the tree takes place starting from
 *  a single SceneNode (typically called root).
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#include <vector>
#include <memory>

#include "Object.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class SceneNode{
public:
    // A SceneNode is created by taking
    // a pointer to an object.
    // For now, we also specify the shader paths as well (TODO: Implement a shader manager here
    //                                                          instead for a cleaner code..
    SceneNode(std::shared_ptr<Object> ob, std::string vertShader, std::string fragShader);
    // Our destructor takes care of destroying
    // all of the children within the node.
    // Now we do not have to manage deleting
    // each individual object.
    ~SceneNode();
    // Adds a child node to our current node.
    void AddChild(SceneNode* n);
    // Draws the current SceneNode
    void Draw();
    // Updates the current SceneNode
    void Update(glm::mat4 projectionMatrix, Camera* camera);
    // Returns the local transformation transform
    // Remember that local is local to an object, where it's center is the origin.
    Transform& GetLocalTransform();
    // Returns a SceneNode's world transform
    Transform& GetWorldTransform();
    // For now we have one shader per Node.
    std::shared_ptr<Shader> m_shader; 
    
    // NOTE: Protected members are accessible by anything
    // that we inherit from, as well as ?
protected:
    // Parent
    SceneNode* m_parent;
private:
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    std::vector<SceneNode*> m_children;
    // The object stored in the scene graph
    std::shared_ptr<Object> m_object;
    // Each SceneNode nodes locals transform.
    Transform m_localTransform;
    // We additionally can store the world transform
    Transform m_worldTransform;
};

#endif
