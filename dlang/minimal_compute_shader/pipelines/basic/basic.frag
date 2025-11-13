#version 430 core

in vec2 TexCoords;
uniform sampler2D tex;
out vec4 color;

void main()
{
	vec3 texCol = texture(tex, TexCoords).rgb;
	color = vec4(texCol, 1.0f);
}
