
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

#include "lodepng.h"
#include "shaderprogram.h"
#include"Colliders.h"
#include<cmath>
#include<string>
#include<fstream>
#include<iostream>
#include <sstream>

glm::vec3 BoxCollider::GetCenter()
{
    return center;
}
float BoxCollider::GetX()
{

    return size_x;
}
float BoxCollider::GetY()
{

    return size_y;
}
float BoxCollider::GetZ()
{

    return size_z;
}

BoxCollider::BoxCollider(glm::vec3 c, float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
    center=c;
    size_x=maxx-minx;
    size_y=maxy-miny;
    size_z=maxz-minz;
    center.x=center.x+minx;
    center.y=center.y+miny;
    center.z=center.z+minz;

}
BoxCollider::BoxCollider()
{
    center=glm::vec3(0.0f,0.0f,0.0f);
    size_x=0;
    size_y=0;
    size_z=0;

}
bool BoxCollider::isCollisionDetected(glm:: vec3 plane_center, float radius)
{
    // Collision x-axis?
    bool collisionX = center.x + size_x >= plane_center.x &&
                      plane_center.x +radius >= center.x;
    bool collisionY = center.y + size_y >= plane_center.y &&
                      plane_center.y + radius >= center.y;
    bool collisionZ = center.z + size_z >= plane_center.z &&
                      plane_center.z + radius >= center.z;


    return (collisionX && collisionY&&collisionZ);
}
SphereCollider::SphereCollider() {};
SphereCollider::SphereCollider(glm::vec3 c, float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
    center=c;
    radius=fmax(fmax(maxx,maxy),maxz);
    center.x=center.x+minx;
    center.y=center.y+miny;
    center.z=center.z+minz;

}
float SphereCollider::GetRadius()
{

    return radius;
}

glm::vec3 SphereCollider::GetCenter()
{
    return center;
}


