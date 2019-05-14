#include "Object.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

#include "lodepng.h"
#include "shaderprogram.h"


#include<string>
#include<fstream>
#include<iostream>
#include <sstream>

using namespace std;
//===============================================================================


    Object::Object(string ObjPath, string TexPath)
    {
    LoadModel(ObjPath);
    tex=readTexture(TexPath);
    }
    void Object::Draw(ShaderProgram *sp, glm::mat4 P,  glm::mat4 V,  glm::mat4 M)
    {
    float *ver= out_vertices.data();
    float *nor=out_normals.data();
    float *uv=out_uvs.data();
    int c=0;
 /*   for(int i=0;i<=out_vertices.size();i++)
    {
        cout<<ver[i]<<",";
c++;
if(c==4){cout<<endl;c=0;}

    }
    getch(); */

std::copy(out_vertices.begin(), out_vertices.end(), ver);
std::copy(out_normals.begin(), out_normals.end(), nor);
std::copy(out_uvs.begin(), out_uvs.end(), uv);




    int vertexCount=out_vertices.size()/4;
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

    glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,ver); //Wska¿ tablicê z danymi dla atrybutu vertex

    glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
    glVertexAttribPointer(sp->a("normal"),4,GL_FLOAT,false,0,nor); //Wska¿ tablicê z danymi dla atrybutu normal

    glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
    glVertexAttribPointer(sp->a("color"),4,GL_FLOAT,false,0,uv); //Wska¿ tablicê z danymi dla atrybutu color

    glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt
    glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
    glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
    glDisableVertexAttribArray(sp->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color

    }

 // virtual ~ Object ();
void Object::LoadModel(string path)
    {
                vector< glm::vec3 > temp_vertices;
    vector< glm::vec2 > temp_uvs;
    vector< glm::vec3 > temp_normals;
        file.open(path.c_str());
        if(file.good())
        {
            cout<<"File opened:"<<path<<endl;

            while(!file.eof())
            {
                string first;
                file>>first;
              //  cout<<first<<endl;
               if(first=="v") //wierzcholki
                  {

                   glm::vec3 vertex;
                    file>>vertex.x>>vertex.y>>vertex.z;

                    temp_vertices.push_back(vertex);
                    }
              else if( first=="vt") {//wierzcholki z uv
                    glm::vec2 uv;
                    file>>uv.x>>uv.y;
                    temp_uvs.push_back(uv);
                    }
                else if(first=="vn"){ //normale
                    glm::vec3 normal;
                    file>>normal.x>>normal.y>>normal.z;
                    temp_normals.push_back(normal);
                    }
                else if(first=="f"){//face

                    string vertexIndex[3], uvIndex[3], normalIndex[3];
                    string info[3];

                    file>>info[0]>>info[1]>>info[2];
                    for(int j=0;j<3;j++){
                    int p=0;
                    for(int i=0;i<=info[j].size();i++)
                    {
                        if((p==0)&&info[j][i]!='/') vertexIndex[j]=vertexIndex[j]+info[j][i];
                       else if((p==1)&&info[j][i]!='/') uvIndex[j]=uvIndex[j]+info[j][i];
                        else if((p==2)&&info[j][i]!='/') normalIndex[j]=normalIndex[j]+info[j][i];

                        else{
                            p++;
                        }


                    }

                    }

                    vertexInd.push_back(strtoint(vertexIndex[0]));
                    vertexInd.push_back(strtoint(vertexIndex[1]));
                    vertexInd.push_back(strtoint(vertexIndex[2]));
                    uvInd    .push_back(strtoint(uvIndex[0]));
                    uvInd    .push_back(strtoint(uvIndex[1]));
                    uvInd    .push_back(strtoint(uvIndex[2]));
                    normalInd.push_back(strtoint(normalIndex[0]));
                    normalInd.push_back(strtoint(normalIndex[1]));
                    normalInd.push_back(strtoint(normalIndex[2]));
                   }

              else{


              }




              //  file>>first; //pozbywamy sie konca wiersza
             //   cout<<first<<endl;
 }
cout<<"Temporary vectors ready\n";
            for( unsigned int i=0; i<vertexInd.size(); i++ )
            {
                unsigned int vertexIndex = vertexInd[i];
                glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
               // cout<<vertexIndex-1<<":"<<vertex.x<<" "<<vertex.y<<" "<<vertex.z<<endl;

                out_vertices.push_back(vertex.x);
                out_vertices.push_back(vertex.y);
                out_vertices.push_back(vertex.z);
                out_vertices.push_back(1.0f);

            }
            for( unsigned int i=0; i<uvInd.size(); i++ )
            {
                unsigned int uvIndex = uvInd[i];
                glm::vec2 uv = temp_uvs[ uvIndex-1 ];
                out_uvs.push_back(uv.x);
                out_uvs.push_back(uv.y);

            }
            for( unsigned int i=0; i<normalInd.size(); i++ )
            {
                unsigned int normalIndex = normalInd[i];
                glm::vec3 normal = temp_normals[ normalIndex-1 ];

                 out_normals.push_back(normal.x);
                out_normals.push_back(normal.y);
                out_normals.push_back(normal.z);
                 out_normals.push_back(0.0f);
            }
/*int c=0;
            for(int i=0;i<out_normals.size();i++)
            {

                cout<<out_normals[i]<<",";
c++;
if(c==4){cout<<endl;c=0;}
            }//*/

            cout<<"Model loaded successful"<<endl;
        }
        else{
            cout<<"Couldn't open file:"<<path<<endl;
        }
        file.close();

    }
GLuint Object::readTexture(string filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);

  //Wczytanie do pamięci komputera
  std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
  unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
  //Wczytaj obrazek
  unsigned error = lodepng::decode(image, width, height, filename);

  //Import do pamięci karty graficznej
  glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
  glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
  //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return tex;
}
unsigned int Object::strtoint(string s)
{
unsigned int i;
std::istringstream istr(s);

istr >> i;

    return i;
}



