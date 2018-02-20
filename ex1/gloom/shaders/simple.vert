#version 430 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv_vert;

out vec2 uv;

void main()
{
    uv = uv_vert;
    gl_Position = vec4(position, 1.0f);
}
