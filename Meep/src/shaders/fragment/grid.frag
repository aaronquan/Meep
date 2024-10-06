#version 330 core

uniform vec2 u_resolution;
uniform vec2 grid_size;
uniform float u_time;

uniform sampler1D ourTexture;

float circle(in vec2 _st, in float _radius){
    vec2 l = _st-vec2(0.5);
    return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(l,l)*4.0);
}

void main() {

	vec2 st = gl_FragCoord.xy/u_resolution;
    vec3 color = vec3(0.0);

    st *= grid_size;      // Scale up the space by 3
    //st.x *= grid_size.x;
    st = fract(st); // Wrap around 1.0

    // Now we have 9 spaces that go from 0-1

    color = vec3(st,0.0);
    //color = vec3(circle(st,0.5));
    vec4 textureColour = texture(ourTexture, st.x);
	gl_FragColor = vec4(vec3(textureColour), 1.0);
}
