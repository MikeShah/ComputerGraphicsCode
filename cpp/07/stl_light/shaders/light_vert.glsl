#version 410 core

layout(location=0) in vec3 position;

uniform mat4 u_MVP;

void main()
{
    vec4 newPosition = u_MVP* vec4(position,1.0f);

		// Compute MVP matrix for light
    gl_Position = vec4(newPosition.x,newPosition.y,newPosition.z,newPosition.w);
}


