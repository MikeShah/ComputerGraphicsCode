// @file: interface.glsl
// An interface block starts with a 'qualifier' such as
// 'in', 'out', or 'uniform'

// All things found in interface block 'block' are uniforms
uniform myBlock{
  // List of variables in interface block.
  // ...
  vec4 v1;
  vec4 v2;
  vec4 v3;
  bool boolean;
};

// A 'named interface' has a name 'SomeName' that acts
// as a 'namespace' for the variables defined within the 
// interface block
uniform namedBlock{
  vec4 v4;
}SomeName;


struct Lights{
  vec3 position;
  vec3 color;
};

uniform GlobalSceneData{
  mat4 viewMatrix;
  mat4 projectionMatrix;
  Light mLights[40];
}scene;


void main(){
  // I can access thinks in 'myBlock' as:
  // v1, v2, v3, boolean witihn main().
  v1 = vec4(1.0,1.0,1.0,1.0);

  // I can access things in the namedBlock by the name
  // 'SomeName'.
  SomeName.v4 = vec4(0.25,0.25,0.5,1.0);
}



