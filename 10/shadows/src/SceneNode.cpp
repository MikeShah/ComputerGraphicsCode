#include "SceneNode.hpp"

#include <string>
#include <iostream>

// The constructor
SceneNode::SceneNode(std::shared_ptr<Object> ob, std::string vertShader, std::string fragShader){
	std::cout << "(SceneNode.cpp) Constructor called\n";
	m_object = ob;

    // By default no parent.
    m_parent = nullptr;
	
    // Create shader
    m_shader = std::make_shared<Shader>();
	// Setup shaders for the node.
	std::string vertexShader   = m_shader->LoadShaderAsString(vertShader);
	std::string fragmentShader = m_shader->LoadShaderAsString(fragShader);

	// Actually create our shader
	m_shader->CreateShader(vertexShader,fragmentShader);       
}

// The destructor 
SceneNode::~SceneNode(){
    // Remove all of the children
    for(int i=0; i < m_children.size(); i++){
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
	m_shader->Bind();
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
    
        m_object->Bind();
    	// Now apply our shader 
		m_shader->Bind();
    	// Set the uniforms in our current shader

        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        m_shader->SetUniform1i("u_DiffuseMap",0);  
        // TODO: This assumes every SceneNode is a 'Terrain' so this shader setup code
        //       needs to be moved preferably to 'Object' or 'Terrain'
        m_shader->SetUniform1i("u_DetailMap",1);  
        // Set the MVP Matrix for our object
        // Send it into our shader
        m_shader->SetUniformMatrix4fv("model", &m_worldTransform.GetInternalMatrix()[0][0]);
        m_shader->SetUniformMatrix4fv("view", &camera->GetWorldToViewmatrix()[0][0]);
        m_shader->SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

        // Create a 'light'
        // Create a first 'light'
        m_shader->SetUniform3f("pointLights[0].lightColor",1.0f,1.0f,1.0f);
        m_shader->SetUniform3f("pointLights[0].lightPos",
           camera->GetEyeXPosition() + camera->GetViewXDirection(),
           camera->GetEyeYPosition() + camera->GetViewYDirection(),
           camera->GetEyeZPosition() + camera->GetViewZDirection());
        m_shader->SetUniform1f("pointLights[0].ambientIntensity",0.9f);
        m_shader->SetUniform1f("pointLights[0].specularStrength",0.5f);
        m_shader->SetUniform1f("pointLights[0].constant",1.0f);
        m_shader->SetUniform1f("pointLights[0].linear",0.003f);
        m_shader->SetUniform1f("pointLights[0].quadratic",0.0f);
		
		// Create a second light
        m_shader->SetUniform3f("pointLights[1].lightColor",1.0f,0.0f,0.0f);
        m_shader->SetUniform3f("pointLights[1].lightPos",
           camera->GetEyeXPosition() + camera->GetViewXDirection(),
           camera->GetEyeYPosition() + camera->GetViewYDirection(),
           camera->GetEyeZPosition() + camera->GetViewZDirection());
        m_shader->SetUniform1f("pointLights[1].ambientIntensity",0.9f);
        m_shader->SetUniform1f("pointLights[1].specularStrength",0.5f);
        m_shader->SetUniform1f("pointLights[1].constant",1.0f);
        m_shader->SetUniform1f("pointLights[1].linear",0.09f);
        m_shader->SetUniform1f("pointLights[1].quadratic",0.032f);

	
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
