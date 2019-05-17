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
class Collider
{
public:
    Collider(glm::vec3 c, float x, float y, float z);
    Collider();
    bool isCollisionDetected(glm:: vec3 plane_center, float X, float Y, float Z);
    glm::vec3 GetCenter();
    float GetX();
    float GetY();
    float GetZ();
private:
    glm::vec3 center;
    float size_x;
    float size_y;
    float size_z;

};
class Object
{
public:
    Object();
    Object(string ObjPath, const char* TexPath);
    void Draw(ShaderProgram *sp, glm::mat4 P,  glm::mat4 V,  glm::mat4 M);


private:
    vector< unsigned int > vertexInd, uvInd, normalInd;

    vector < float >  out_vertices;
    vector < float >  out_uvs;
    vector < float >  out_normals;
    ifstream file;
    GLuint tex;
    void LoadModel(string path);
    GLuint readTexture(const char* filename);
    unsigned int strtoint(string s);

};
