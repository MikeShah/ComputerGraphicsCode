#version 410 core

layout(location=0) in vec3 position;

uniform vec3 u_LightPos;
uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;

void main()
{
    vec4 newPosition = u_Perspective*u_ModelMatrix* vec4(u_LightPos,1.0f);

    // Directly position light in world space
    gl_Position = vec4(newPosition.x,newPosition.y,newPosition.z,1.0f);
}


