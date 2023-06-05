#ifndef MESHMAKER_HPP
#define MESHMAKER_HPP

#include <unordered_map>
#include <vector>
#include <cassert>
#include <memory>

#include "Vertex.hpp"

#include <iostream>
// The purpose of this class is to make it easy to
// generate new meshes by specifying the positions
// of vertices.
// 

// TODO: 
//      - Make vertices normalized?
//      -     
class MeshMaker{
    public:
        // Specify the vertex layout for the mesh
        // at creation time
        MeshMaker(VERTEXLAYOUTS layout){
            m_Layout = layout;
        }
        ~MeshMaker(){
			//de-allocate all resources once they've outlived their purpose:
			glDeleteVertexArrays(1, &m_VAOId);
			glDeleteBuffers(1, &m_vertexPositionBuffer);
			glDeleteBuffers(1, &m_vertexPositionBuffer);
        }

        // Create specific primitives
        void CreateTexturedPlane(float width, float depth){
            // positions, 3D normal, 2D texture coordinate
            // x,y,z,     nx,ny,nz,  s,t
            std::shared_ptr<Vertex3TN> v0 = std::make_shared<Vertex3TN>( width, -0.5f,  depth,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f);
            std::shared_ptr<Vertex3TN> v1 = std::make_shared<Vertex3TN>(-width, -0.5f,  depth,   0.0f, 1.0f, 0.0f, 0.0f,  1.0f);
            std::shared_ptr<Vertex3TN> v2 = std::make_shared<Vertex3TN>(-width, -0.5f, -depth,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
            std::shared_ptr<Vertex3TN> v3 = std::make_shared<Vertex3TN>( width, -0.5f, -depth,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

            m_Layout = VERTEXLAYOUTS::VERTEX3TN;
            AddTriangle(v0,v1,v2);
            AddTriangle(v0,v2,v3);
            Generate();
        }

        void CreateCube(float x, float y, float z){
            // back face
            std::shared_ptr<Vertex3TN> back0 = std::make_shared<Vertex3TN>(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f); // bottom-left
            std::shared_ptr<Vertex3TN> back1 = std::make_shared<Vertex3TN>( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f); // bottom-right 
            std::shared_ptr<Vertex3TN> back2 = std::make_shared<Vertex3TN>( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f); // top-right
            std::shared_ptr<Vertex3TN> back3 = std::make_shared<Vertex3TN>(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f); // top-left
            AddTriangle(back0,back2,back1);
            AddTriangle(back2,back0,back3);
            // front face
            std::shared_ptr<Vertex3TN> f0 = std::make_shared<Vertex3TN>(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f); // bottom-left
            std::shared_ptr<Vertex3TN> f1 = std::make_shared<Vertex3TN>( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f); // bottom-right
            std::shared_ptr<Vertex3TN> f2 = std::make_shared<Vertex3TN>( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f); // top-right
            std::shared_ptr<Vertex3TN> f3 = std::make_shared<Vertex3TN>(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f); // top-left
            AddTriangle(f0,f1,f2);
            AddTriangle(f2,f3,f0);
            // left face
            std::shared_ptr<Vertex3TN> l0 = std::make_shared<Vertex3TN>(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f); // top-right
            std::shared_ptr<Vertex3TN> l1 = std::make_shared<Vertex3TN>(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f); // top-left
            std::shared_ptr<Vertex3TN> l2 = std::make_shared<Vertex3TN>(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f); // bottom-left
            std::shared_ptr<Vertex3TN> l3 = std::make_shared<Vertex3TN>(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f); // bottom-right
            AddTriangle(l0,l1,l2);
            AddTriangle(l2,l3,l0);
            // right face
            std::shared_ptr<Vertex3TN> r0 = std::make_shared<Vertex3TN>( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f); // top-left
            std::shared_ptr<Vertex3TN> r1 = std::make_shared<Vertex3TN>( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f); // bottom-right
            std::shared_ptr<Vertex3TN> r2 = std::make_shared<Vertex3TN>( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f); // top-right         
            std::shared_ptr<Vertex3TN> r3 = std::make_shared<Vertex3TN>( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f); // bottom-left     
            AddTriangle(r0,r1,r2);
            AddTriangle(r1,r0,r3);
            // bottom face
            std::shared_ptr<Vertex3TN> b0 = std::make_shared<Vertex3TN>(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f); // top-right
            std::shared_ptr<Vertex3TN> b1 = std::make_shared<Vertex3TN>( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f); // top-left
            std::shared_ptr<Vertex3TN> b2 = std::make_shared<Vertex3TN>( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f); // bottom-left
            std::shared_ptr<Vertex3TN> b3 = std::make_shared<Vertex3TN>(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f); // bottom-right
            AddTriangle(b0,b1,b2);
            AddTriangle(b2,b3,b0);
            // top face
            std::shared_ptr<Vertex3TN> t0 = std::make_shared<Vertex3TN>(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f); // top-left
            std::shared_ptr<Vertex3TN> t1 = std::make_shared<Vertex3TN>( 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f); // bottom-right
            std::shared_ptr<Vertex3TN> t2 = std::make_shared<Vertex3TN>( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f); // top-right     
            std::shared_ptr<Vertex3TN> t3 = std::make_shared<Vertex3TN>(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f); // bottom-left
            AddTriangle(t0,t1,t2);
            AddTriangle(t1,t0,t3);

            m_Layout = VERTEXLAYOUTS::VERTEX3TN;
            Generate();
        }
    
        // Add a vertex to the mesh
        //  Returns 1 if new vertex added
        //  Returns index if the vertex has already been
        //  added
        int AddTriangle(std::shared_ptr<Vertex3TN> v1, std::shared_ptr<Vertex3TN> v2, std::shared_ptr<Vertex3TN> v3){
            // Indices that will make up the triangle
            // We check if they already exist first,
            // Then we will add them.
            int i1= -1;
            int i2= -1;
            int i3= -1;

            // Check to see if each vertex exists,
            // otherwise add it
            if(m_vertexMap.contains(v1)){
                i1 = m_vertexMap[v1];
            }
            else{
                i1 = m_vertexMap.size();
                std::pair<std::shared_ptr<Vertex3TN>,int> pair;
                pair.first=v1;
                pair.second=i1;
                m_vertexMap.insert(pair);
                m_vertices.push_back(v1->x);
                m_vertices.push_back(v1->y);
                m_vertices.push_back(v1->z);
                m_vertices.push_back(v1->nx);
                m_vertices.push_back(v1->ny);
                m_vertices.push_back(v1->nz);
                m_vertices.push_back(v1->s);
                m_vertices.push_back(v1->t);
            } 
            
            if(m_vertexMap.contains(v2)){
                i2 = m_vertexMap[v2];
            }else{
                i2 = m_vertexMap.size();
                std::pair<std::shared_ptr<Vertex3TN>,int> pair;
                pair.first=v2;
                pair.second=i2;
                m_vertexMap.insert(pair);
                m_vertices.push_back(v2->x);
                m_vertices.push_back(v2->y);
                m_vertices.push_back(v2->z);
                m_vertices.push_back(v2->nx);
                m_vertices.push_back(v2->ny);
                m_vertices.push_back(v2->nz);
                m_vertices.push_back(v2->s);
                m_vertices.push_back(v2->t);
            } 

            if(m_vertexMap.contains(v3)){
                i3 = m_vertexMap[v3];
            }else{
                i3 = m_vertexMap.size();
                std::pair<std::shared_ptr<Vertex3TN>,int> pair;
                pair.first=v3;
                pair.second=i3;
                m_vertexMap.insert(pair);
                m_vertices.push_back(v3->x);
                m_vertices.push_back(v3->y);
                m_vertices.push_back(v3->z);
                m_vertices.push_back(v3->nx);
                m_vertices.push_back(v3->ny);
                m_vertices.push_back(v3->nz);
                m_vertices.push_back(v3->s);
                m_vertices.push_back(v3->t);
            } 
            
            // Finally make a triangle by adding to the
            // indices
            // First check to confirm that the index
            // was indeed set
            assert(i1 >=0 && "illegal i1 index");
            assert(i2 >=0 && "illegal i2 index");
            assert(i3 >=0 && "illegal i3 index");
            
            // A triangle is made by pushing back 3
            // indices of either newly created vertices,
            // or otherwise, referencing previously
            // created indices found in our map
            m_indices.push_back(i1);
            m_indices.push_back(i2);
            m_indices.push_back(i3);
        } 
 
        // Functions for working with individual vertices
        unsigned int GetBufferSizeInBytes(){
            int vertexlayoutsize= 0;

            if(m_Layout == VERTEXLAYOUTS::VERTEX2){
                vertexlayoutsize = sizeof(Vertex2);
            }
            else if(m_Layout == VERTEXLAYOUTS::VERTEX3TN){
                vertexlayoutsize = sizeof(Vertex3TN);
            }

            return vertexlayoutsize * m_indices.size();
        }

        // Retrieve the Buffer Data Size
        unsigned int GetVerticesCount(){
            return m_vertices.size();            
        }
    
        // Retrieve the Buffer Data Pointer
        float* GetBufferDataPtr(){
            return m_vertices.data();    
        }

        unsigned int GetIndicesCount(){
            return m_indices.size();
        }

        unsigned int* GetIndicesDataPtr(){
            return m_indices.data();
        }


        void PrintVertices(){
            for(int i=0; i < m_vertices.size(); i++){
                std::cout << m_vertices[i] << ", ";
                if(m_Layout == VERTEXLAYOUTS::VERTEX3TN){
                    if(i%8==0){
                        std::cout << std::endl;
                    }
                }
            }
        }

        
        void PrintIndices(){
            for(int i=0; i < m_indices.size(); i++){
                std::cout << m_indices[i] << ", ";
                if(i%3==0 && m_Layout == VERTEXLAYOUTS::VERTEX3TN){
                    std::cout << std::endl;
                }
            }
        }


        // A normal map layout needs the following attributes
        //
        // positions: x,y,z
        // texcoords: s,t
        // normals:  x,y,z
        void Generate(){
            m_stride = 8;          // TODO: Update based on layout 
            // VertexArrays
            glGenVertexArrays(1, &m_VAOId);
            glBindVertexArray(m_VAOId);

            // Vertex Buffer Object (VBO)
            glGenBuffers(1, &m_vertexPositionBuffer); // selecting the buffer is
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex3TN), m_vertices.data(), GL_STATIC_DRAW);

            // Setup Vertex Buffer attributes
            // Add three floats for x,y,z position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*m_stride, 0);
            // Add three floats for normal coordinates
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*3));
            // Add two floats for texture coordinates
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*6));
            
            // Setup an index buffer (IBO)
            glGenBuffers(1, &m_indexBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), m_indices.data(),GL_STATIC_DRAW);
        }

		// Draw the mesh
		void RenderMesh(){
		//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glBindVertexArray(m_VAOId);
			glDrawElements(GL_TRIANGLES, m_indices.size(),GL_UNSIGNED_INT,(void*)0); 
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
        
    private:
        // Store the specified vertex layout
        VERTEXLAYOUTS m_Layout;
        // Vertex Array Object
        GLuint m_VAOId;
        // Vertex Buffer
        GLuint m_vertexPositionBuffer;
        // Index Buffer Object
        GLuint m_indexBufferObject;
        // Stride of data (how do I get to the next vertex)
        unsigned int m_stride{0};
        // Store all of the vertices
        // The key is the unique vertex
        // The value is the index number, that
        // is incremented for every new vertice
        // added to the collection
        std::unordered_map<std::shared_ptr<IVertex>,int> m_vertexMap;
        // Each vertex stored in the order that it was inserted
        std::vector<float> m_vertices;
        // The indices for a indexed-triangle mesh
        std::vector<unsigned int> m_indices;
};

#endif
