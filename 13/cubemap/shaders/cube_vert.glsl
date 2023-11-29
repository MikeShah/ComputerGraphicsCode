#version 410 core
layout(location=0) in vec3 position;

out vec3 vTexCoords;

/// Note: No model matrix needed
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main(){
    vTexCoords = position;

    gl_Position = u_Projection * u_ViewMatrix* vec4(position, 1.0);
}
