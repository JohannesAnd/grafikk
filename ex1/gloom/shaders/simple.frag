#version 430 core

in vec2 uv;
layout(binding=0) uniform sampler2D mySampler;
out vec4 color;

void main()
{
    color = texture(mySampler, uv);
}
