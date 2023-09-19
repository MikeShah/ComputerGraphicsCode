// @file ooc.cpp
// Object-Oriented 'C' programming
// Overly simplistic example to help you understand how OpenGL works
#include <iostream>

// Some type of OpenGL object with a unique handle and
// some data (i.e. 'array of bytes')
struct VertexBufferObject{
    int     handle;
    void*   data;
};

// Global OpenGL context
// Keeps track of all 'state' and 'objects'
struct glContext{
    VertexBufferObject VBO[100];// Assume we can only have 100 objects
    int currentFreeHandle    =0;    // Keeps track of next free identifier
                                // and increments by 1 every time we
                                // add a new object.
};

// Allocate a global for the context
glContext globalContext;


/// handle is like a unique 'id'
int GenerateVertexBufferObject(int* handle,void* data){
    // OpenGL context automatically finds a free 'id' or 'handle'
    // to allocate a new object.
    int current = globalContext.currentFreeHandle;
    *handle = current;
    
    // Assign this unique handle to our object.
    globalContext.VBO[current].handle = current;
    globalContext.VBO[current].data   = data;

    // Increment
    globalContext.currentFreeHandle++;

    return 0; // Return 0 for success, or some error code.
}

int useVBO(int handle){
    globalContext.VBO[handle].data; // Do something with data
    std::cout << "Doing something with handle..." << std::endl;

    return 0;
}

int main(){

    int someHandle1;
    int someHandle2;
    int someHandle3;
    GenerateVertexBufferObject(&someHandle1, NULL);
    GenerateVertexBufferObject(&someHandle2, NULL);
    GenerateVertexBufferObject(&someHandle3, NULL);

    std::cout << "someHandle3 = " << someHandle3 << std::endl;

    return 0;
}
