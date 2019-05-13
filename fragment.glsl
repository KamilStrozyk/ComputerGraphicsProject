#version 330


//Zmienne interpolowane
in vec4 ic;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

void main(void) {
	pixelColor=ic;
}
