#ifndef VERTEX_HPP
#define VERTEX_HPP

// Keep track of the layouts
enum class VERTEXLAYOUTS {VERTEX2,VERTEX3TN,END};

// Base class for Vertex
struct IVertex{
    // Must implement '==' operator
    virtual bool operator==(const IVertex& rhs){
        return false;
    };
};

// Vertex 2
// 2 - The number of positions
struct Vertex2 : public IVertex{
    float x,y;
    bool operator==(const Vertex2& rhs) const{
        if(x==rhs.x && y==rhs.y){
           return true;
        }
        return false;
    }
};

// Vertex 3TN
// 3 - The number of positions
// T - Includes Texture Coordinates
// N - Includes Normals
struct Vertex3TN : public IVertex{
    float x,y,z;
    float nx,ny,nz;
    float s,t;
	// Constructor
	Vertex3TN(float _x, float _y, float _z, float _s, float _t):
			x(_x),y(_y),z(_z),s(_s),t(_t){
			}
	Vertex3TN(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _s, float _t):
			x(_x),y(_y),z(_z),nx(_nx),ny(_ny),nz(_nz),s(_s),t(_t){
			}
    bool operator==(const Vertex3TN& rhs) const{
        if(x==rhs.x && y==rhs.y && z==rhs.z &&
           s==rhs.s && t==rhs.t &&
           nx==rhs.nx && ny==rhs.ny && nz==rhs.nz){
           return true;
        }
        return false;
    }
};


#endif
