#version 330 core

out vec4 FragColor;

uniform float rotation;
uniform vec2 direction;

in vec2 position;

float circle(in vec2 _st, in vec2 centre, in float _radius){
    float radius = _radius*_radius;
    vec2 dist = _st-centre;
	return 1.-smoothstep(radius-(radius*0.01),
                         radius+(radius*0.01),
                         dot(dist,dist)*4.0);
}

void main()
{
    vec2 p = vec2(cos(rotation), sin(rotation))*0.4+vec2(0.5);
    //vec2 p = (direction*0.4)+vec2(0.5);
    float circ2 = circle(position, p, 0.2);

    float circ = circle(position, vec2(0.5), 1.0);
    vec3 colour = vec3(0.6, 0.2, 0.3)*circ2+vec3(0.3, 0.4, 1)*circ;
	FragColor = vec4(vec3(colour), circ+circ2);
}