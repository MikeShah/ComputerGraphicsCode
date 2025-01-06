// Abbreviated version of an OpenGL Object
struct gl_buffer_object
{
   GLuint Name;
   GLubyte *Data;
   /* ... other fields ... */
};

// 'target' is the 'buffer type'
//          e.g. GL_ARRAY_BUFFER for a VBO
//
// 'buffer' is the name (represented by an integer)
//          of our gl_buffer_object we are using.
void glBindBuffer(GLenum target, GLuint buffer){
	// Do something here with the 'struct gl_buffer_object'
	// ...
	// ...
}
