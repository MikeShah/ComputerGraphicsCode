#version 410 core

// should match 'patch parameter'
layout(vertices = 4) out;

void main(){
  // gl_InvocationID: Index of the current vertex within this patch
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  gl_TessLevelOuter[0] = 2; // Left   for quads
  gl_TessLevelOuter[1] = 2; // Bottom for quads
  gl_TessLevelOuter[2] = 2; // Right  for quads
  gl_TessLevelOuter[3] = 2; // Top    for quads

  gl_TessLevelInner[0] = 2; // up/down for quads
  gl_TessLevelInner[1] = 2; // left/right for quads

}
