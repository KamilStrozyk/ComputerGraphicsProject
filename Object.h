#ifndef _Object_
#define _Object_
#endif // _Object_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "shaderprogram.h"


#include<string>
#include<fstream>
#include<vector>
#include <sstream>
using namespace std;

class Object
{
public:
    Object();
    Object(string ObjPath, string TexPath);
    void Draw(ShaderProgram *sp, glm::mat4 P,  glm::mat4 V,  glm::mat4 M);
private:
    vector< unsigned int > vertexInd, uvInd, normalInd;

    vector < float >  out_vertices;
    vector < float >  out_uvs;
    vector < float >  out_normals;
    ifstream file;
    GLuint tex;
    void LoadModel(string path);
    GLuint readTexture(string filename);
    unsigned int strtoint(string s);

};
