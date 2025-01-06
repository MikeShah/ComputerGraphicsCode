#include "Camera.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

void Camera::MouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);

    // Little hack for our 'mouse look function'
    // We need this so that we can move our camera
    // for the first time.
    static bool firstLook=true;
    if(true == firstLook){
        firstLook=false;
        m_oldMousePosition = newMousePosition;
    }

    glm::vec2 delta = m_oldMousePosition - newMousePosition;

    m_viewDirection = glm::rotate(m_viewDirection,glm::radians(delta.x),m_upVector);
 
    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;
}


void Camera::MoveForward(float speed){
    //    m_eyePosition.z -= speed;
        m_eyePosition.x += m_viewDirection.x *speed;
        m_eyePosition.y += m_viewDirection.y *speed;
        m_eyePosition.z += m_viewDirection.z *speed;
}

void Camera::MoveBackward(float speed){
   //    m_eyePosition.z += speed;
        m_eyePosition.x -= m_viewDirection.x *speed;
        m_eyePosition.y -= m_viewDirection.y *speed;
        m_eyePosition.z -= m_viewDirection.z *speed;
}

void Camera::MoveLeft(float speed){
}

void Camera::MoveRight(float speed){
}

void Camera::MoveUp(float speed){
}

void Camera::MoveDown(float speed){
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


Camera::Camera(){
    std::cout << "Camera.cpp: (Constructor) Created a Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 3.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
