#version 330 core

uniform float u_time;
uniform vec2 u_resolution;

uniform sampler2D tex2;
uniform sampler1D tex;

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.01, pct, st.y) -
          smoothstep( pct, pct+0.01, st.y);
}

float plot_texture_red_channel(vec2 st, sampler1D tex_id){
    vec4 c = texture(tex, st.x);
    float y = c.r;
    return plot(st, y);
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    float pt = plot_texture_red_channel(st, tex);

    vec4 c2 = texture(tex2, st);

    vec3 colour = pt*vec3(0.0,0.0,1.0)+c2.r*vec3(1.0, 1.0, 0.0)*pt;

    gl_FragColor = vec4(colour, 1.0);
}