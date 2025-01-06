#version 410 core

in vec2 v_textureCoordinates;

// Setup our texture Map.
// Recall that textures are uniform.
uniform sampler2D u_DiffuseTexture;


out vec4 color;

// Entry point of program
void main()
{
	vec3 diffuseColor = vec3(0.0f,0.0f,0.0f);
    
	diffuseColor = texture(u_DiffuseTexture, v_textureCoordinates).rgb;

    // zero out the alpha value if #ff00ff color is used anywhere on our texture
    float alpha = 1.0f;

    if(diffuseColor.r == 1.0f && diffuseColor.b == 1.0f){
        alpha = 0.0f;
    }

	// Output color based on our texture
	color = vec4(diffuseColor,alpha);
}
