#version 410 core

in vec3 vTexCoords;

out vec4 color;

uniform samplerCube u_skybox;

void main(){

    color = texture(u_skybox, vTexCoords); 
}
