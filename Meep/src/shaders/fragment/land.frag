#version 330 core

in vec2 position;

out vec4 FragColor;

uniform vec3 land_colour;
uniform vec3 water_colour;

uniform vec2 u_resolution;
uniform sampler1D water_levels;

uniform float unit_size;

vec4 getColour(vec3 colour);

float waterLevelsTexture(vec2 st, sampler1D tex_id);

void main(){
	vec2 st = position.xy;
	float water_area = 0.2;

	//float s = step(0.2, st.y);
	FragColor = getColour(land_colour)*waterLevelsTexture(st, water_levels);
}

vec4 getColour(vec3 colour){
    return vec4(colour,1.0);
}

float waterLevelsTexture(vec2 st, sampler1D tex_id){
	float water_size = 1 - unit_size;
	float half_size = unit_size/2;
    vec4 c = texture(tex_id, (st.x*water_size)+half_size);
    float y = c.r;
    return step(y, st.y);
}