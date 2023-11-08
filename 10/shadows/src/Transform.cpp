#include "Transform.hpp"

// By default, all transform matrices
// are also identity matrices
Transform::Transform(){
    LoadIdentity();
}

Transform::~Transform(){

}

// Resets the model transform as the identity matrix.
void Transform::LoadIdentity(){
    m_modelTransformMatrix = glm::mat4(1.0f);
}

void Transform::Translate(float x, float y, float z){
        // Here we apply our camera
        // This first transformation is applied to the object
        // This is the model transform matrix.
        // That is, 'how do I move our model'
        // Here we see I have translated the model -1.0f away from its original location.
        // We supply the first argument which is the matrix we want to apply
        // this transformation to (Our previous transformation matrix.
        m_modelTransformMatrix = glm::translate(m_modelTransformMatrix,glm::vec3(x,y,z));                            
}

void Transform::Rotate(float radians, float x, float y, float z){
    m_modelTransformMatrix = glm::rotate(m_modelTransformMatrix, radians,glm::vec3(x,y,z));        
}

void Transform::Scale(float x, float y, float z){
    m_modelTransformMatrix = glm::scale(m_modelTransformMatrix,glm::vec3(x,y,z));        
}

// Returns the actual transform matrix
// Useful for sending 
GLfloat* Transform::GetTransformMatrix(){
    return &m_modelTransformMatrix[0][0];
}


// Get the raw internal matrix from the class
glm::mat4 Transform::GetInternalMatrix() const{
    return m_modelTransformMatrix;
}

void Transform::ApplyTransform(Transform t){
    m_modelTransformMatrix = t.GetInternalMatrix();
}


// Perform a matrix multiplication with our Transform
Transform& Transform::operator*=(const Transform& t) {
    m_modelTransformMatrix =  m_modelTransformMatrix * t.GetInternalMatrix();
    return *this;
}

// Perform a matrix addition with our Transform
Transform& Transform::operator+=(const Transform& t) {
    m_modelTransformMatrix =  m_modelTransformMatrix + t.GetInternalMatrix();
    return *this;
}

// Matrix assignment
Transform& Transform::operator=(const Transform& t) {
    m_modelTransformMatrix =  t.GetInternalMatrix();
    return *this;
}

// Perform a matrix multiplication with our Transform
//       x * y should return a copy, rather than a reference
//       need to be very careful when operator overloading.
//       See operator*= for an example of returning the reference
//       and avoiding the copy.
// This code probably needs optimized (lots of new objects on the stack!)
Transform operator*(const Transform& lhs, const Transform& rhs){
    Transform result;

    result.m_modelTransformMatrix = lhs.GetInternalMatrix() * rhs.GetInternalMatrix();

    return result;
}

// Transform Addition
Transform operator+(const Transform& lhs, const Transform& rhs){
    Transform result;

    result.m_modelTransformMatrix = lhs.GetInternalMatrix() + rhs.GetInternalMatrix();

    return result;
}
