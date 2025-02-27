#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoords;
layout(location=2) in vec3 normals;

out vec3 vWorldPos;
out vec2 vTexCoords;
out vec3 vNormals;

void main()
{
	vWorldPos  = position;
	vTexCoords = texcoords;
	vNormals   = normals;
	gl_Position = vec4(position.x, position.y, position.z, 1.0f);
}
