// @file: object3d.d
// NOTE: I named this 'object3d.d' because 'object.d' is a file
//       used in the D language for classes.

struct Object3D{
	GLuint mVAO;
	GLuint mVBO;
	GLUint mIBO;

	GLUint mShaderID;

	Texture[] mTextures;

	mat4 mModelMatrix;
}
