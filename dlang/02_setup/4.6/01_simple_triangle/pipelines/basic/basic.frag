#version 450 core

in vec3 v_vertexColors;
layout(location=0) out vec4 color;

void main()
{
	color = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);

}
