/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"

#include<ctime>
#include "myCube.h"
#include<string>
#include<fstream>
#include<iostream>
#include <sstream>
#include<conio.h>
#include "Object.h"
#include "Colliders.h"
#include<cmath>
using namespace std;


//===============================================================================


enum Phase
{
    Idle=0,
    Start =1,
    Flight=2,
    Landing=3,
    End=4,
};
//===============================================================================
Phase phase=Flight;
float cloudPos=-100.0f;
float Plane2Pos=-10.0f;
float pos_y=0;
float pos_z=0;
float pos_x=0;
float speed=0;
float speed_z=0;
float speed_y=0;
 float angle_z=0; //Aktualny k¹t obrotu obiektu
    float angle_y=0; //Aktualny k¹t oborotu obiektu
    float skybox_angle=0;
float aspectRatio=1;
float angle_plane=0.0f;
ShaderProgram *sp;
ShaderProgram *diffuse;
BoxCollider cols[144];
BoxCollider GroundCollider;
SphereCollider PlaneCollider;
Object * Buildings[7];
//===============================================================================
//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
    if (action==GLFW_PRESS)
    {
        switch(phase)
        {
        case Flight:
            if (key==GLFW_KEY_LEFT)
                speed_z=-PI/6;
            if (key==GLFW_KEY_RIGHT)
                speed_z=PI/6;
            if (key==GLFW_KEY_UP)
                speed_y=PI/6;
            if (key==GLFW_KEY_DOWN)
                speed_y=-PI/6;

            if(key==GLFW_KEY_W)
                speed+=addSpeed;
            if(key==GLFW_KEY_S)
                speed=-addSpeed;

            break;
        default:
            break;


        }
        if(key==GLFW_KEY_ESCAPE)
            phase=End;
    }
    if (action==GLFW_RELEASE)
    {
        if (key==GLFW_KEY_LEFT)
            speed_z=0;
        if (key==GLFW_KEY_RIGHT)
            speed_z=0;
        if (key==GLFW_KEY_UP)
            speed_y=0;
        if (key==GLFW_KEY_DOWN)
            speed_y=0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int pos_y)
{
    if (pos_y==0)
        return;
    aspectRatio=(float)width/(float)pos_y;
    glViewport(0,0,width,pos_y);
}
void WriteConstColliders(Object * G, Object * P)
{
    int i=0;
for(int x=0;x<12;x++)
{
    for(int y=0;y<12;y++)
    {



   cols[i]=BoxCollider(glm::vec3(x*10.0f,-2.0f,y*10.0f),Buildings[i%2]->GetMinX(),Buildings[i%1]->GetMinY(),Buildings[i%1]->GetMinZ(),Buildings[i%1]->GetMaxX(),Buildings[i%1]->GetMaxY(),Buildings[i%1]->GetMaxZ());
    i++;
    }

}
    GroundCollider=BoxCollider(glm::vec3(0.0f,-1.0f,0.0f),G->GetMinX(),G->GetMinY(),G->GetMinZ(),G->GetMaxX(),G->GetMaxY(),G->GetMaxZ());
    PlaneCollider=SphereCollider(glm::vec3(0.0f,-1.0f,0.0f),P->GetMinX(),P->GetMinY(),P->GetMinZ(),P->GetMaxX(),P->GetMaxY(),P->GetMaxZ());


}
//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window)
{
    //************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
    glClearColor(0.5,0.8,1,1); //kolor nieba TODO: Skybox
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window,windowResizeCallback);
    glfwSetKeyCallback(window,keyCallback);

    sp=new ShaderProgram("vertex.glsl",NULL,"fragment.glsl");

       diffuse=new ShaderProgram("v_textured.glsl",NULL,"f_textured.glsl");
}

//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window)
{
    //************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

    delete sp;
    delete diffuse;
}

void Reset()
{

       pos_x=0;
       pos_y=0;
       pos_z=0;
 angle_z=0;
     angle_y=0;
       speed=0;
angle_plane=0;
}


//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window,float angle_z,float angle_y, Object * O, Object * G, Object * C, Object * P2, Object * S)
{
     sp->use();
   //  diffuse->use();
  glUniform4f(sp->u("lp1"),0,0,-6,1); //Współrzędne źródła światła
  glUniform4f(sp->u("lp2"),34,34,-66,0.1); //Współrzędne źródła światła
    //************Tutaj umieszczaj kod rysuj¹cy obraz******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 M,V,P;
//===============================================================================
//Pozycja Kamery

    V=glm::lookAt(
          glm::vec3(pos_x-10.0f, pos_y, pos_z-15),
          glm::vec3(pos_x-10.0f,pos_y,pos_z-10.0f),
          glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku

    P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 500.0f); //Wylicz macierz rzutowania

    V=glm::rotate(V,angle_y,glm::vec3(-0.5f,0.0f,0.0f));
    V=glm::rotate(V,angle_plane,glm::vec3(0.0f,0.5f,0.0f));


//===============================================================================
//===============================================================================
//pozostale obiekty

    M=glm::mat4(1.0f);
    M=glm::translate(M, glm::vec3(0.0f,-2.0f,0.0f));

    G->Draw(sp,P,V,M);
   M=glm::scale(M, glm::vec3(100.0f,100.0f,100.0f));



//rysowanie budnykow
for(int x=0;x<12;x++)
{
    for(int y=0;y<12;y++)
    {

    M=glm::mat4(1.0f);
    M=glm::translate(M, glm::vec3(x*10.0f,-2.0f,y*10.0f));
  //   M=glm::scale(M, glm::vec3(2.0f,2.0f,2.0f));
    Buildings[y%2]->Draw(sp,P,V,M);
    }

}


for(int i=0;i<= 40;i++)
{
    M=glm::mat4(1.0f);
M=glm::translate(M,glm::vec3(cloudPos+ i*50,10.0f, i*50));
M=glm::scale(M,glm::vec3(1.5,1.5,1.5));
  C->Draw(sp,P,V,M);
}
    M=glm::mat4(1.0f);
M=glm::translate(M,glm::vec3(-10.0f,8.0f, Plane2Pos));
P2->Draw(sp,P,V,M);
//Pozycja samolotu

    M=glm::mat4(1.0f);
    // if(phase==Flight&& flightSpeed<=0)
  //   M=glm::rotate(M,angle_z,glm::vec3(1.0f,0.0f,0.0f));
 // M=glm::rotate(M,angle_z,glm::vec3(0.0f,1.0f,0.0f));
     glm::vec3 center(pos_x-10.0f,pos_y,pos_z-10.0f);

    M=glm::rotate(M,angle_plane,glm::vec3(0.0f,-1.0f,0.0f));
    M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f));
    M=glm::translate(M, glm::vec3(pos_x-10.0f,pos_y,pos_z-10.0f));
    // gluLookAt(0, pos_y,pos_z -5,0,pos_y,pos_z, 0, 1, 0);
    M=glm::rotate(M,angle_z,glm::vec3(0.0f,0.0f,1.0f));
    M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f));
     //  V=glm::translate(V, glm::vec3(0.0f,pos_y,pos_z));
    //Wylicz macierz modelu
    //Wylicz macierz modelu

    center = glm::vec3(M * glm::vec4(center, 1.0));
//środek masy



//===============================================================================
//Kolizje

  for(int i=0; i<144; i++)
    {
    if(cols[i].isCollisionDetected(center,PlaneCollider.GetRadius()))
    {
    Reset();
   // break;
    }
    }
    if(GroundCollider.isCollisionDetected(center,PlaneCollider.GetRadius())) Reset();
   // else cout<<"GROUND NOT DETECTED\n";
   // cout<<pos_x<<";"<<pos_y<<";"<<pos_z<<";"<<PlaneCollider.GetRadius()<<":::"<<cols[30].GetX()<<";"<<cols[30].GetY()<<";"<<cols[30].GetZ()<<endl;
    //getch();
//===============================================================================

 O->Draw(sp,P,V,M);
diffuse->use();
M=glm::mat4(1.0f);
M=glm::scale(M,glm::vec3(200,200,200));
  M=glm::rotate(M,skybox_angle,glm::vec3(0.0f,1.0f,0.0f));
S->Draw(diffuse,P,V,M);
    glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni
}


int main(void)
{

//===============================================================================
//wczyrywanie modeli
    GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

    glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

    if (!glfwInit())   //Zainicjuj bibliotekê GLFW
    {
        fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(700, 700, "Samolot", NULL, NULL);
    if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
    {
        fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
    glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

    if (glewInit() != GLEW_OK)   //Zainicjuj  const char * bibliotekê GLEW
    {
        fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Operacje inicjuj¹ce
    Object Plane2("Resources/Models/Plane.obj","Resources/Tex/plane2.png");
    Object Plane("Resources/Models/Plane.obj","Resources/Tex/plane.png");
    Object Cube("Resources/Models/skybox.obj","Resources/Tex/Skybox/plane.png");
    Object Cloud("Resources/Models/clouds.obj","Resources/Tex/plane.png");
    Object Ground("Resources/Models/ground.obj","Resources/Tex/ground.png");
    for(int i=0;i<2;i++)
    {
          const char * tex="Resources/Tex/budynki.png";
        string  path="Resources/Models/budynek";
        char no= '0'+i%7+1;
        path=path+no;
        path=path+".obj";
     //Object x(path,tex);
 Buildings[i]= new Object(path,tex);



    }

    WriteConstColliders(&Ground,&Plane);

//===============================================================================
    //G³ówna pêtla
//    float angle_z=0; //Aktualny k¹t obrotu obiektu
  //  float angle_y=0; //Aktualny k¹t oborotu obiektu

    glfwSetTime(0); //Zeruj timer

    while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
    {
        cloudPos+=2*glfwGetTime();
        Plane2Pos+=15*glfwGetTime();
        skybox_angle+=PI/36*0.1*glfwGetTime();
        switch(phase)
        {
        case Idle:
            break;
        case Start:
            break;
        case Flight:
            angle_z+=speed_z*glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
            angle_plane+=speed_z*glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
            if(angle_z>= angle_z_max)
                angle_z=angle_z_max;
            if(angle_z<= angle_z_min)
                angle_z=angle_z_min;
            if(angle_y>= angle_y_max)
                angle_y=angle_y_max;
            if(angle_y<= angle_y_min)
                angle_y=angle_y_min;
            angle_y+=speed_y*glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
            pos_z+=speed*glfwGetTime();
            pos_x+=30*-angle_z*(Cz*p*S*speed*speed/2-g*m)/(2*m)*glfwGetTime()*glfwGetTime();
            pos_y+=/*30*sin(angle_y)*/(Cz*p*S*speed*speed/2-g*m)/(2*m)*glfwGetTime()*glfwGetTime();//- g*glfwGetTime();
            if(pos_x-10.0f<-10.0f||pos_x-10.0f>150.0f||pos_z-10.0f<-50.0f||pos_z-10.0f>170.0f) Reset();
            break;
        case Landing:
            break;
        default:
            freeOpenGLProgram(window);

            glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
            glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
            exit(EXIT_SUCCESS);
            break;
        }
        glfwSetTime(0); //Zeruj timer
        drawScene(window,angle_z,angle_y,&Plane,&Ground,&Cloud,&Plane2,&Cube); //Wykonaj procedurê rysuj¹c¹
        glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.

    }
//===============================================================================
    freeOpenGLProgram(window);

    glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
    exit(EXIT_SUCCESS);
//===============================================================================
}
