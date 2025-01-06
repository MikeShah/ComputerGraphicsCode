#version 410 core
// From Vertex Buffer Object (VBO)
// The only thing that can come 'in', that is
// what our shader reads, the first part of the
// graphics pipeline.
layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexNormals;
layout(location=2) in vec3 vertexColors;

// Uniform variables
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection; // We'll use a perspective projection

// Uniform Light Variables
uniform vec3 u_LightPos;

// Pass vertex colors into the fragment shader
out vec3 v_vertexColors;
out vec3 v_vertexNormals;
out vec3 v_worldSpaceFragment;

void main()
{
    // Send to fragment shader the normals
    // and the vertex colors
    v_vertexColors = vertexColors;
    v_vertexNormals= vertexNormals;

    // Calculate in world space the position of the vertex
    v_worldSpaceFragment = vec3( u_ModelMatrix*vec4(position,1.0f));

    // Compute the MVP matrix
    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);
}


