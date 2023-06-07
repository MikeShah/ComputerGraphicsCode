/** @file Transform.hpp
 *  @brief Responsible for holding matrix operations in model, view, and projection space..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// The purpose of this class is to store
// transformations of 3D entities (cameras, objects, etc.)
class Transform{
public:

    // Constructor for a new transform
    Transform();
    // Destructor for the transform
    ~Transform();
    // Resets matrix transformations to the identity matrix
    void LoadIdentity();
    // Perform a translation of an object
    void Translate(float x, float y, float z);
    // Perform rotation about an axis
    void Rotate(float radians, float x, float y, float z);
    // Perform rotation about an axis
    void Scale(float x, float y, float z);
    // Returns the transformation matrix
    GLfloat* GetTransformMatrix();
    // Apply Transform
    // Takes in a transform and sets internal
    // matrix.
    void ApplyTransform(Transform t);
    // Returns the transformation matrix
    glm::mat4 GetInternalMatrix() const;

    // Transform multiplication t1 *= t2 (t1 is multiplied and a new result stored)
	Transform& operator*=(const Transform& t);
	// Transform addition
	Transform& operator+=(const Transform& t);
	// Transform =
	Transform& operator=(const Transform& t);
    // Multiplication operator overload
    // Returns a new copy of the result of 
    // t3 = t1 * t2. (Returned value is stored in 't3' which is returned)
    friend Transform operator*(const Transform& lhs, const Transform& rhs);
    // Addition operator overload
    // Returns a new copy of the result of 
    // t3 = t1 + t2. (Returned value is stored in 't3' which is returned)
    friend Transform operator+(const Transform& lhs, const Transform& rhs);

private:
    // Stores the actual transformation matrix
    glm::mat4 m_modelTransformMatrix;
};


#endif
