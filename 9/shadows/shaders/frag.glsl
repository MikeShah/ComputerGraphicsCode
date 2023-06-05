// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Our light source data structure
struct PointLight{
    vec3 lightColor;
    vec3 lightPos;
    float ambientIntensity;

    float specularStrength;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[2];

// Used for our specular highlights
uniform mat4 view;


// Import our normal data
in vec3 myNormal;
// Import our texture coordinates from vertex shader
in vec2 v_texCoord;
// Import the fragment position
in vec3 FragPos;

// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
// Load in an additional detail map
//uniform sampler2D u_DetailMap; 

void main()
{
    // Compute the normal direction
    vec3 norm = normalize(myNormal);
    
    // Store our final texture color
    vec3 diffuseColor   = texture(u_DiffuseMap, v_texCoord).rgb;
//    vec3 detailColor    = texture(u_DetailMap,  v_texCoord).rgb;

	// Store our final lighting computation
	vec3 Lighting = vec3(0.0,0.0,0.0);

	// TODO: (Optional) You should refactor this into a separate function :)
	for(int i=0; i < 1; i++){
		// (1) Compute ambient light
		vec3 ambient = pointLights[i].ambientIntensity * pointLights[i].lightColor;

		// (2) Compute diffuse light
		// From our lights position and the fragment, we can get
		// a vector indicating direction
		// Note it is always good to 'normalize' values.
		vec3 lightDir = normalize(pointLights[i].lightPos - FragPos);
		// Now we can compute the diffuse light impact
		float diffImpact = max(dot(norm, lightDir), 0.0);
		vec3 diffuseLight = diffImpact * pointLights[i].lightColor;

		// (3) Compute Specular lighting
		vec3 viewPos = vec3(0.0,0.0,0.0);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = pointLights[i].specularStrength * spec * pointLights[i].lightColor;

		// Calculate Attenuation here
		// distance and lighting... 
		float distance = length(pointLights[i].lightPos - FragPos);
		float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance*distance));

		ambient 		*= attenuation;
		diffuseLight 	*= attenuation;
		specular 		*= attenuation;


		// Our final color is now based on the texture.
		// That is set by the diffuseColor
		Lighting += diffuseLight + ambient + specular;
	}

    // Final color + "how dark or light to make fragment"
    if(gl_FrontFacing){
        FragColor = vec4(diffuseColor * Lighting,1.0);
    }else{
        // Additionally color the back side the same color
         FragColor = vec4(diffuseColor * Lighting,1.0);
    }
}

