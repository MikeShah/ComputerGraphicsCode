#version 410 core

in vec3 v_vertexColors;
in vec2 v_textureCoordinates;

// Uniform representing a light
uniform float u_ambientIntensity;
uniform vec3 u_lightColor;


// Setup our texture Map.
// Recall that textures are uniform.
uniform sampler2D u_DiffuseTexture;

out vec4 color;

// Entry point of program
void main()
{
	vec3 diffuseColor = vec3(0.0f,0.0f,0.0f);
	diffuseColor = texture(u_DiffuseTexture, v_textureCoordinates).rgb;

	// Instead of using vertex colors, we will
	// instead output a texture.
	vec4 VertexColors = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);

	// Output color based on our texture and light
	color = vec4(diffuseColor,1.0f) * u_ambientIntensity;
}


