#version 410 core

in  vec3 aColor;
out vec4 color;

void main()
{
	color = vec4(aColor.r,aColor.g,aColor.b, 1.0f);
}
