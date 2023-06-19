#version 410 core
#
// From Vertex Buffer Object (VBO)
// The only thing that can come 'in', that is
// what our shader reads, the first part of the
// graphics pipeline.
layout(location=0) in vec3 position;
layout(location=1) in vec2 textureCoordinates;

// Uniform variables
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
// We'll use a perspective projection
uniform mat4 u_Projection; 


// Pass texture coordinates to the fragment shader
out vec2 v_textureCoordinates;
// For our particle simulation
uniform int u_particleFrame;
uniform int  u_dimensionsX;
uniform int  u_dimensionsY;

void main()
{	
    
	// The 'animatedCoordinates' are modifying the original textureCoordinates to 
	// 'shrink' and select a smaller portion of a texture
    vec2 animatedCoordinates = textureCoordinates;
    // Based on the frame, select a smaller texture coordinate within the texture
    // First we 'divide' by the dimensions for the x and y coorindate.
    float xBoxSize = 1.0f / u_dimensionsX;
    float yBoxSize = 1.0f / u_dimensionsY;
    // Scale our coordinates down to a 'box'
    animatedCoordinates.x *= xBoxSize;
    animatedCoordinates.y *= yBoxSize;
    // Then effectively select the frame we want
    animatedCoordinates.x += (u_particleFrame % u_dimensionsX) * xBoxSize;
    animatedCoordinates.y += (u_particleFrame / u_dimensionsY) * yBoxSize;

	v_textureCoordinates = animatedCoordinates;

    vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);
                                                                    // Don't forget 'w'
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}


