#version 410 core

out vec4 color;

in vec2 vTexCoords; 
uniform sampler2D sampler1;

void main()
{

		color = vec4(texture(sampler1,vTexCoords).rgb,1.0);
//	color = vec4(1.0f,0.0f,0.0f, 1.0f);
}
