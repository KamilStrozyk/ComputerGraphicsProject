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
class BoxCollider
{
public:
    BoxCollider(glm::vec3 c, float minx, float miny, float minz, float maxx, float maxy, float maxz);
    BoxCollider();
    bool isCollisionDetected(glm:: vec3 plane_center, float radius);
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
class SphereCollider
{
public:
    SphereCollider(glm::vec3 c, float minx, float miny, float minz, float maxx, float maxy, float maxz);
    SphereCollider();
    glm::vec3 GetCenter();
    float GetRadius();

private:
    glm::vec3 center;
    float radius;


};






