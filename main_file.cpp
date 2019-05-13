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
#include "myCube.h"
#include "myTeapot.h"

#include<string>
#include<fstream>
#include<iostream>
#include <sstream>
#include<conio.h>
#include "Object.h"
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
float addSpeed=10;
float speed;
float speed_z=0;
float speed_y=0;
float aspectRatio=1;
ShaderProgram *sp;
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
                speed_z=-PI/2;
            if (key==GLFW_KEY_RIGHT)
                speed_z=PI/2;
            if (key==GLFW_KEY_UP)
                speed_y=PI/2;
            if (key==GLFW_KEY_DOWN)
                speed_y=-PI/2;

            if(key==GLFW_KEY_W)
                speed=addSpeed;
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

void windowResizeCallback(GLFWwindow* window,int width,int height)
{
    if (height==0)
        return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
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

}


//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window)
{
    //************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

    delete sp;
}




//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window,float angle_z,float angle_y, float flightSpeed,float pos, Object * O)
{
    //************Tutaj umieszczaj kod rysuj¹cy obraz******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V=glm::lookAt(
                    glm::vec3(0, 0, -5),
                    glm::vec3(0,0,0),
                    glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);
    if(phase==Flight&& flightSpeed<=0)
        M=glm::translate(M, glm::vec3(0.0f,1.0f,0.0f));
    M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
    M=glm::rotate(M,angle_z,glm::vec3(0.0f,0.0f,1.0f)); //Wylicz macierz modelu



    sp->use();//Aktywacja programu cieniuj¹cego
    //Przeslij parametry programu cieniuj¹cego do karty graficznej
O->Draw(sp,P,V,M);
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

    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

    if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
    {
        fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
    glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

    if (glewInit() != GLEW_OK)   //Zainicjuj bibliotekê GLEW
    {
        fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Operacje inicjuj¹ce
     Object Plane("Resources/Models/cube.obj","Resources/Models/TALTS.jpg");

//===============================================================================
    //G³ówna pêtla
    float angle_z=0; //Aktualny k¹t obrotu obiektu
    float angle_y=0; //Aktualny k¹t obrotu obiektu
    float flightSpeed=0;
    float height=0;
    float pos=0;
    glfwSetTime(0); //Zeruj timer

    while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
    {
        switch(phase)
        {
        case Idle:
            break;
        case Start:
            break;
        case Flight:
            angle_z+=speed_z*glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
            angle_y+=speed_y*glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
            flightSpeed+=speed*glfwGetTime(); //dodaj predkosci samolotu
            pos+=flightSpeed;
            height+=flightSpeed;
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
        drawScene(window,angle_z,angle_y, flightSpeed,pos,&Plane); //Wykonaj procedurê rysuj¹c¹
        glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.

    }
//===============================================================================
    freeOpenGLProgram(window);

    glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
    exit(EXIT_SUCCESS);
//===============================================================================
}
