#ifndef STL_OBJECT_HPP
#define STL_OBJECT_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include "util.hpp"

#include "globals.hpp"

// The STL format is a simple 'triangle soup' format that outputs
// one normal per triangle, and then the following three triangles
// The .stl bunny was obtained from: https://en.m.wikipedia.org/wiki/File:Stanford_Bunny.stl
// And then modified in blender3D.
struct STLFile{
	private:
		std::string mFilepath;
		std::vector<float> mVertices;
		std::vector<float> mNormals;
		std::vector<float> mColors;

		GLuint mVAO= 0;
		GLuint mVBO[3];

		GLuint mShaderID	= 0;
	public:
		STLFile();
		~STLFile();
		void Initialize();
        void PreDraw();
		void Draw();
		std::vector<float> GetVertices() const;
		std::vector<float> GetNormals() const;
};


#endif
