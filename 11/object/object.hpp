#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object3D{
public:    
    Object3D(){
   
    }
    ~Object3D(){
    
    } 
    void Render(){
        /* Select Object buffers */
        glBindVertexArray(mVertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIndexBufferObject);
        /* Setup Object State */
        glEnable(GL_TEXTURE_2D);
        glUseProgram(gGraphicsPipelineShaderProgram);
        /* Setup uniforms */
        /* ... */

        /* Setup uniforms - textures */
		gTexture.Bind(0);

        /* Issue draw call */
        glDrawElements(GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    0);

        // Clear state
        glUseProgram(0);
        glBindVertexArray(0);
    }
private: 
    // OpenGL Buffer IDs
    GLuint mVertexArrayObject	;
    GLuint mVertexBufferObject	;
    GLuint mIndexBufferObject  ;
    // ShaderID for rendering object
    GLuint mShaderID= 0;
    // Texture IDs or Texture Abstraction for our object
    std::vector<Texture> mTextures;

    // Transformations
    glm::mat4 mModelMatrix;
};

#endif
