#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp1; //Współrzędne źródła światła w przestrzeni świata
uniform vec4 lp2; //Współrzędne źródła światła w przestrzeni świata

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wspolrzedne wektora normalnego w przestrzeni modelu
in vec2 texCoord0; //współrzędna teksturowania

//Zmienne interpolowane
out vec4 l1;
out vec4 l2;
out vec4 n;
out vec4 v;

out vec2 iTexCoord0;


void main(void) {
    l1=normalize(V*(lp1-M*vertex)); //Wektor "do światła" w przestrzeni oka
    l2=normalize(V*(lp2-M*vertex)); //Wektor "do światła" w przestrzeni oka
    n=normalize(V*M*normal); //Wektor normalny w przestrzeni oka
    v=normalize(vec4(0,0,0,1)-V*M*vertex); //Wektor "do obserwatora" w przestrzeni oka

    iTexCoord0=texCoord0; //Zapewnienie interpolacji współrzędnych teksturowania podanych poprzez atrybut


    gl_Position=P*V*M*vertex;
}
