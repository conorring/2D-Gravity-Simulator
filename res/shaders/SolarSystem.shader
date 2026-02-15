#shader vertex
#version 330 core
layout(location=0) in vec2 vertex_position;
layout(location=1) in vec2 instance_position;

uniform mat4 projection;

void main()
{   
    vec2 actual_position = 0.15*vertex_position + instance_position;
    gl_Position = projection*vec4(actual_position, 0.0, 1.0);
}
