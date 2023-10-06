#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

struct vertex{
    float x,y,z;
    float nx,ny,nz;
};

struct Mesh{
    // Other strategy is to have std::vector<vertex> vertices
    std::vector<float> positions;
    std::vector<float> normals;

    Mesh(std::string filename);

};


#endif
