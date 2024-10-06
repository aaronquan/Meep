#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 position;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    position = aCoord;
}