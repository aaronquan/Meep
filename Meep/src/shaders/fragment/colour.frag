#version 330 core
out vec4 FragColor;

uniform vec3 colour;

vec4 getColour();

void main(){
	FragColor = getColour();
}

vec4 getColour(){
    return vec4(colour,1.0);
}