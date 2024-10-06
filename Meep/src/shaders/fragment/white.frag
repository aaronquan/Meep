#version 330 core
out vec4 FragColor;

vec4 white();

void main(){
	FragColor = white();
}

vec4 white(){
    return vec4(1.0,1.0,1.0,1.0);
}