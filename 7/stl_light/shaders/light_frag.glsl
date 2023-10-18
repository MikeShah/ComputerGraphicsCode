#version 410 core
// Uniform Light Variables
uniform vec3 u_LightPos;

out vec4 color;

// Entry point of program
void main()
{
    // Send fragment to output
	color = vec4(1.0f,0.0f,0.0f, 1.0f);
}
