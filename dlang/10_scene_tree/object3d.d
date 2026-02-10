// @file: object3d.d
// NOTE: I named this 'object3d.d' because 'object.d' is a file
//       used in the D language for classes.
//       Other good names might be 'Entity3D', 'Actor3D', etc.

// Added purely so that this code can compile.
alias GLuint = uint;
alias mat4   = float[16];
alias Texture = uint;

struct Object3D{
  // Handles to named opengl objects
	GLuint mVAO;
	GLuint mVBO;
	GLuint mIBO;
  // Shader/material handle
	GLuint mShaderID;

  // Transformations
	mat4 mModelMatrix;

  // Texture references
	Texture[] mTextures;




  void DoSomeTransformation(){}
}


void main(){

  Object3D myObject1;
  Object3D myObject2;
  Object3D myObject3;
  Object3D myObject4;
  Object3D myObject5;
  Object3D myObject6;
  Object3D myObject7;
  Object3D myObject8;
  Object3D myObject9;
  Object3D myObject10;
  // etc.


 
}
