#version 410 core

// Uniform Light Variables
uniform vec3 u_LightPos;

in vec3 v_vertexNormals;
in vec3 v_vertexColors;
in vec3 v_worldSpaceFragment;

out vec4 color;

// Entry point of program
void main()
{
    // Normalize every attribute incoming
    // from vertex shader because they are
    // interpolated values
    vec3 normals  = normalize(-v_vertexNormals); 
    vec3 lightDirection = normalize(u_LightPos - v_worldSpaceFragment);

    // Compute the diffuse component by taking the dot product of the incoming light direction
    float diffuse =  max(0.0,dot(lightDirection,normals));

    // Compute the final lighting
    vec3 Lighting = v_vertexColors * (diffuse);
	
    // Send fragment to output
		color = vec4(Lighting.x,Lighting.y,Lighting.z, 1.0f);
}
