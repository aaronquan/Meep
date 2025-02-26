#version 330 core
out vec4 FragColor;

in vec2 position;

uniform vec4 outline_colour;
uniform vec2 circle_centre;
uniform float radius;
uniform float outline_width;

void main(){
	float half_width = outline_width*0.5;
	float dist_from_outline = abs(radius - distance(circle_centre, position));

	float in_outline = step(dist_from_outline, half_width);
	vec4 colour = in_outline*outline_colour+(1.0-in_outline)*vec4(0.0, 0.0, 0.0, 1.0);

	FragColor = colour;
}