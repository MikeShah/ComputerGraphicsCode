// Example tessellation shader for a quad
#version 410 core

//
//
// ccw - default winding order
layout(quads, equal_spacing, ccw) in;

void main(){

float u = gl_TessCoord.x;
float v = gl_TessCoord.y;

vec4 pos0 = gl_in[0].gl_Position;
vec4 pos1 = gl_in[1].gl_Position;
vec4 pos2 = gl_in[2].gl_Position;
vec4 pos3 = gl_in[3].gl_Position;

vec4 leftPos = pos0 + v * (pos3-pos0);
vec4 rightPos= pos1 + v * (pos2-pos1);
vec4 pos = leftPos+ u*(rightPos - leftPos);

 gl_Position = pos; // Matrix transformations additionally go here.

}
