#version 330 core
out vec4 FragColor;

in vec2 position;

uniform vec4 circle_colour;
uniform vec4 background_colour;
uniform vec2 circle_centre;
uniform float radius;

void main(){
	float circle = step(distance(circle_centre, position), radius);
	float not_circle = 1.0 - circle;

	vec4 colour = circle*circle_colour + not_circle*background_colour;

	FragColor = colour;
}