#version 410 core

in vec3 v_vertexColors;

out vec4 color;

// Entry point of program
void main()
{
	color = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);
}
