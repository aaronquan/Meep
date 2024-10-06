#version 330 core
#define M_PI 3.1415926535897932384626433832795


uniform vec2 u_resolution;
uniform float u_time;

vec3 colourA = vec3(0.149,0.141,0.912);
vec3 colourB = vec3(1.000,0.833,0.224);

out vec4 FragColour;

float plot(vec2 st, float pct) {    
    return smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

float easeInSine(float x) {
  return 1 - cos((x * M_PI) / 2);
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution;

	float y = smoothstep(0, 1, easeInSine(st.x));

	float pct = plot(st, easeInSine(y));

	vec3 colour = mix(colourA, colourB, y);
	colour = (1.0-pct)*colour+pct*vec3(0.0,1.0,0.0);

	FragColour = vec4(colour, 1.0);
}