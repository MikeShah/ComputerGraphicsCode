/// This example is to help understand how 'object oriented C' works in regards to OpenGL and buffer binding.
/// Effectively, this should helpy you underestand that only one buffer of any type is allowed to be bound
/// before a draw call.
/// In Object-Oriented C, our tools are 'handles' to represent each object, and enums, which can represent
/// one 'state' or one type of 'handle' that is currently bound.
import std.stdio;

/// Enums are effectively a way to create 'objects' in OpenGL
enum BufferTarget{BUFFER_ARRAY, ELEMENT_ARRAY};
enum BufferUsage{STATIC,DYNAMIC};

/// Global object for OpenGL
struct GlobalOpenGLContext{
    // Allocate a bunch of bufferes
    // In this 'simple model' assume that index of Buffer corresponds to the bufferID.
    // (i.e. buffer[1] corresponds to bufferID of 1.
    static Buffer[10] mBuffers;

    // OpenGL Binds to at most '1' of each buffer type at a given time
    static uint         bufferArrayCurrentHandle;
    static uint         elementArrayCurrentHandle;
}

void PrintGlobalContextInfo(){
    // Buffers
    foreach(buf ; GlobalOpenGLContext.mBuffers){
        writeln(buf);  
    }
    writeln;
    // Currently bound buffers
    writeln("Array Buffer Handle currently bound:  ",GlobalOpenGLContext.bufferArrayCurrentHandle);
    writeln("Elemetn Array Handle currently bound: ",GlobalOpenGLContext.elementArrayCurrentHandle);
}

/// A chunk of data 'somewhere
struct Buffer{
    static s_nextAvailableID=1;
    
    int bufferId;
    void* data;
    size_t sizeOfData;
    BufferUsage usage;
}

/// n - Number of buffers
/// buffers - passed in as a pointer, and 'returns'
///           a pointer to the allocated buffers from the function.
//   NOTE:
//   GenBuffer(size_t n,out uint buffers) would be the equivalent
//   semantics as using a 'pointer'.
//   However, I choose to avoid a dlang feature here, and match the 'C style'
//   as it will match the documentation you otherwise serach.
void GenBuffer(size_t n,uint* buffers){
    // This function will 'mark' some number of buffers
    // as reserved for your use.
    // You as a user get an 'integer handle' that uniquely identifies
    // which data (in some table maintained by OpenGL) you are using.
    
    // Increment or otherwise 'find' the next avaialble free id
    uint nextID = Buffer.s_nextAvailableID;
    
    // Update the bufferID in the Context
    GlobalOpenGLContext.mBuffers[nextID].bufferId = nextID;

    // Increment the next ID
    Buffer.s_nextAvailableID++;

    // Return the value
    *buffers = nextID;
}

// 'Select' which buffer to associate with a handle
void BindBuffer(BufferTarget target, uint bufferHandle){
    
    switch(target){
        case BufferTarget.BUFFER_ARRAY:
            GlobalOpenGLContext.bufferArrayCurrentHandle = bufferHandle;
            break;
        case BufferTarget.ELEMENT_ARRAY:
            GlobalOpenGLContext.elementArrayCurrentHandle= bufferHandle;
            break;
        default:
            writeln("Error, invalid handle type");
    }
}

// Set the buffer data for whatever handle is currently 'binded/bound'.
void BufferData(BufferTarget target, size_t size, void* data,BufferUsage usage){
    // Assign data 
    Buffer* curBuf;
    switch(target){
        case BufferTarget.BUFFER_ARRAY:
            curBuf = &GlobalOpenGLContext.mBuffers[GlobalOpenGLContext.bufferArrayCurrentHandle];
            break;
        case BufferTarget.ELEMENT_ARRAY:
            curBuf = &GlobalOpenGLContext.mBuffers[GlobalOpenGLContext.elementArrayCurrentHandle];
            break;
         default:
            writeln("Error, bound BufferData to wrong buffer type");
    }

    // Set the data for whatever the buffer type is
    curBuf.data = data;
    curBuf.sizeOfData = size;
    curBuf.usage = usage;
}


void main(){

    ///////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// First Buffer of Data ///////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    uint handleToMyData;
    // Generate 1 or more 'handles' (thus populating 1 or more integers)
    // with unique ids.
    GenBuffer(1,&handleToMyData);
    // Binds exactly 1 'handle' to one 'buffer type'
    // OpenGL only every has exactly '1 ative buffer type' bound at any 
    // given time. You can think of this like 'selecting' the handle
    // for the buffer that you will then continue to work with for
    // all other operations.
    BindBuffer(BufferTarget.BUFFER_ARRAY, handleToMyData);
    // Now for our BufferTarget.BUFFER_ARRAY (bound to handleToMyData), we 
    // set the data.
    int[] vertices = [ 1,1,1,
                       2,2,2,
                       3,3,3];
    BufferData(BufferTarget.BUFFER_ARRAY, vertices.length*int.sizeof, vertices.ptr,BufferUsage.STATIC);

    ///////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// Second Buffer of Data //////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    uint anotherHandle;
    GenBuffer(1,&anotherHandle);
    BindBuffer(BufferTarget.ELEMENT_ARRAY, anotherHandle);
    uint[] indices = [0,1,2];
    BufferData(BufferTarget.ELEMENT_ARRAY, indices.length*uint.sizeof, indices.ptr,BufferUsage.STATIC);


    //////////////////////////// Debug of the buffers ////////////////////////////
    PrintGlobalContextInfo();

}
