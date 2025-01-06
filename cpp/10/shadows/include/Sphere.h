/** @file Sphere.h
 *  @brief Draw a simple sphere primitive.
 *
 *  Draws a simple sphere primitive, that is derived
 *  from the Object class.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#include "Object.h"
#include "Geometry.h"
#include <cmath>

class Sphere : public Object{
public:

    // Constructor for the Sphere
    Sphere();
    // The initialization routine for this object.
    void init();


};

// Calls the initialization routine
Sphere::Sphere(){
    std::cout << "(Sphere.cpp) Sphere constructor called (derived from Object)\n";
    init();
}


// Algorithm for rendering a sphere
// The algorithm was obtained here: http://learningwebgl.com/blog/?p=1253
// Please review the page so you can understand the algorithm. You may think
// back to your algebra days and equation of a circle! (And some trig with
// how sin and cos work
void Sphere::init(){
    unsigned int latitudeBands = 30;
    unsigned int longitudeBands = 30;
    float radius = 1.0f;
    double PI = 3.14159265359;

        for(unsigned int latNumber = 0; latNumber <= latitudeBands; latNumber++){
            float theta = latNumber * PI / latitudeBands;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for(unsigned int longNumber = 0; longNumber <= longitudeBands; longNumber++){
                float phi = longNumber * 2 * PI / longitudeBands;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;
                // Why is this "1-" Think about the range of texture coordinates
                float u = 1 - ((float)longNumber / (float)longitudeBands);
                float v = 1 - ((float)latNumber / (float)latitudeBands);

                // Setup geometry
                geometry.addVertex(radius*x,radius*y,radius*z);   // Position
                geometry.addTexture(u,v);         // Texture
            }
        }

        // Now that we have all of our vertices
        // generated, we need to generate our indices for our
        // index element buffer.
        // This diagram shows it nicely visually
        // http://learningwebgl.com/lessons/lesson11/sphere-triangles.png
        for (unsigned int latNumber1 = 0; latNumber1 < latitudeBands; latNumber1++){
            for (unsigned int longNumber1 = 0; longNumber1 < longitudeBands; longNumber1++){
                unsigned int first = (latNumber1 * (longitudeBands + 1)) + longNumber1;
                unsigned int second = first + longitudeBands + 1;
                geometry.addIndex(first);
                geometry.addIndex(second);
                geometry.addIndex(first+1);

                geometry.addIndex(second);
                geometry.addIndex(second+1);
                geometry.addIndex(first+1);
            }
        }

        // Finally generate a simple 'array of bytes' that contains
        // everything for our buffer to work with.
        geometry.gen();

        // std::cout << "#vertices:" << geometry.getSize() << "\n";
        // std::cout << "#indices:" << geometry.getIndicesSize() << "\n";

        // Create a buffer and set the stride of information
        myBuffer.CreateBufferNormalMapLayout(14,
                                            geometry.getSize(),
                                            geometry.getIndicesSize(),
                                            geometry.getData(),
                                            geometry.getIndicesData());
}
