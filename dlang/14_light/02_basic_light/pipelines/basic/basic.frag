#version 410 core

// Define a light structure
struct Light{
	float mColor[3];
	float mPosition[3];
	float mAmbientIntensity;
	float mSpecularIntensity;
	float mSpecularExponent;
}uLight;

out vec4 color;

in vec3 vWorldPos;
in vec2 vTexCoords; 
in vec3 vNormals; 
uniform sampler2D sampler1;
uniform Light uLight1;

void main()
{
		vec4 diffuseTexture = vec4(texture(sampler1,vTexCoords).rgb,1.0);
		
// Multiply in light source
		color.r = diffuseTexture.r * uLight1.mColor[0]; 
		color.g = diffuseTexture.g * uLight1.mColor[1]; 
		color.b = diffuseTexture.b * uLight1.mColor[2]; 
	
		// Ambient light source
		color += uLight1.mAmbientIntensity;


// Compute difference between light position and the vertex normal
		vec3 lightPosition = vec3(uLight1.mPosition[0],uLight1.mPosition[1],uLight1.mPosition[2]);
		vec3 lightDirection = normalize(vec3(lightPosition - vWorldPos));
		float diffuseAngle = max(dot(lightDirection,vNormals),0.0);
		color *= diffuseAngle;

}
