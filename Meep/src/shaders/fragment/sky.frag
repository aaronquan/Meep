#version 330 core

in vec2 position;

out vec4 FragColor;

uniform vec2 u_resolution;

uniform vec3 sky_colour;
//uniform vec3 cloud_colour;

uniform sampler1D cloud_levels;

vec4 getColour(vec3 colour);

float cloudDrawTexture(vec2 st, sampler1D tex_id);

void main(){
    vec2 st = position.xy;
	FragColor = getColour(sky_colour)+cloudDrawTexture(st, cloud_levels);
}

vec4 getColour(vec3 colour){
    return vec4(colour,1.0);
}

float cloudDrawTexture(vec2 st, sampler1D tex_id){
    int cloudTexSize = textureSize(tex_id, 0);

    vec4 c = texture(tex_id, st.x);
    float y = c.r;
    float d = distance(vec2(st.x, 0.3), st);
    if(st.y > 0.3){
        d *= 3.;
    }
    if(d < y){
        return 1;
    }
    return 0;

    float inc = 1/float(cloudTexSize);

    float s = 0.;

    for(int i = 0; i < cloudTexSize; i++){
        vec4 c = texture(tex_id, (i*inc));
        float y = c.r;

        float d = distance(vec2(i*inc, 0.3), st);
        if(d < y){
            
            s += smoothstep(0.5, 1., 1.-d*3.);
        }
    }

    return s;
}