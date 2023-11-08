#include "SceneNode.hpp"

#include <string>
#include <iostream>

// The constructor
SceneNode::SceneNode(Object* ob){
	std::cout << "(SceneNode.cpp) Constructor called\n";
	m_object = ob;
	// By default, we do not know the parent
	// at the time of construction of this node.
	// If the SceneNode is the root of the tree,
	// then there is no parent.
	m_parent = nullptr;
	
	// Setup shaders for the node.
	std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
	std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
	// Actually create our shader
	m_shader.CreateShader(vertexShader,fragmentShader);       
}

// The destructor 
SceneNode::~SceneNode(){
	// Remove each object
	for(unsigned int i =0; i < m_children.size(); ++i){
		delete m_children[i];
	}
}

// Adds a child node to our current node.
void SceneNode::AddChild(SceneNode* n){
	// For the node we have added, we can set
	// it's parent now to our current node.
	// 'this' is the current instance of our
	// object, which is a pointer to our current
	// SceneNode.
	n->m_parent = this;
	// Add a child node into our SceneNode
	m_children.push_back(n);
}

// Draw simply draws the current nodes
// object and all of its children. This is done by calling directly
// the objects draw method.
void SceneNode::Draw(){
	// Bind the shader for this node or series of nodes
	m_shader.Bind();
	// Render our object
	if(m_object!=nullptr){
		// Render our object
		m_object->Render();
		// For any 'child nodes' also call the drawing routine.
		for(int i =0; i < m_children.size(); ++i){
			m_children[0]->Draw();
		}
	}	
}

// Update simply updates the current nodes
// object. This is done by calling directly
// the objects update method.
// TODO: Consider not passting projection and camera here
void SceneNode::Update(glm::mat4 projectionMatrix, Camera* camera){
    if(m_object!=nullptr){
        // TODO: Implement here!
    
    	// Now apply our shader 
		m_shader.Bind();
    	// Set the uniforms in our current shader

        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        m_shader.SetUniform1i("u_DiffuseMap",0);  
        // Set the MVP Matrix for our object
        // Send it into our shader
        m_shader.SetUniformMatrix4fv("model", &m_worldTransform.GetInternalMatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("view", &camera->GetWorldToViewmatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

        // Create a 'light'
        m_shader.SetUniform3f("lightColor",1.0f,1.0f,1.0f);
        m_shader.SetUniform3f("lightPos",
                               camera->GetEyeXPosition() + camera->GetViewXDirection(),
                               camera->GetEyeYPosition() + camera->GetViewYDirection(),
                               camera->GetEyeZPosition() + camera->GetViewZDirection());
        m_shader.SetUniform1f("ambientIntensity",0.5f);
	
		// Iterate through all of the children
		for(int i =0; i < m_children.size(); ++i){
			m_children[0]->Update(projectionMatrix, camera);
		}
	}
}

// Returns the actual local transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::GetLocalTransform(){
    return m_localTransform; 
}

// Returns the worled  transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::GetWorldTransform(){
    return m_worldTransform; 
}
