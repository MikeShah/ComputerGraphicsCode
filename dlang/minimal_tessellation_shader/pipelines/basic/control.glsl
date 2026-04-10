#version 410 core

// uniform for this demo to tessellation the mesh
uniform int uTessellationLevel;


// should match 'glPatchParameter'
layout(vertices = 4) out;


// This shader is run once per-vertex, just like our vertex shader.
// However, we have all of the data within a 'patch' -- so in this case,
// we have '4' indices that we can access within our primitive patch.

void main(){
  // gl_InvocationID: Index of the current vertex within this patch
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  gl_TessLevelOuter[0] = uTessellationLevel; // Left   for quads
  gl_TessLevelOuter[1] = uTessellationLevel; // Bottom for quads
  gl_TessLevelOuter[2] = uTessellationLevel; // Right  for quads
  gl_TessLevelOuter[3] = uTessellationLevel; // Top    for quads

  gl_TessLevelInner[0] = uTessellationLevel; // up/down for quads
  gl_TessLevelInner[1] = uTessellationLevel; // left/right for quads
}



