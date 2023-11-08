// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0)in vec3 position; 
layout(location=1)in vec2 texCoord; // Our second attribute - texture coordinates.

// If we have texture coordinates we can now use this as well
out vec2 v_texCoord;


void main()
{

    gl_Position = vec4(position.x,position.y,0.0f, 1.0f);

    // Store the texture coordinaets which we will output to
    // the next stage in the graphics pipeline.
    v_texCoord = texCoord;
}
// ==================================================================
