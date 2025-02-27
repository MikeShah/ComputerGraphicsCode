#version 410 core

out vec4 color;

in vec2 vTexCoords; 
uniform sampler2D sampler1;
uniform vec3 uLight1;

void main()
{
		vec4 diffuseTexture = vec4(texture(sampler1,vTexCoords).rgb,1.0);
		
/* 
// Multiply in light source
		color.r = diffuseTexture.r * uLight1.t; 
		color.g = diffuseTexture.g * uLight1.g; 
		color.b = diffuseTexture.b * uLight1.b; 
*/

// Purely additive light below

		color = vec4(texture(sampler1,vTexCoords).rgb,1.0);
		color.r += uLight1.r; 
		color.g += uLight1.g; 
		color.b += uLight1.b; 

}
