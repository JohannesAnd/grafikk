#version 430 core

layout(location=0) in vec3 vert;
layout(location=1) in vec2 vertTextCoord;
layout(location=2) in vec3 vertNormal;

layout(binding=2) uniform sampler2D animationSampler;

uniform mat4 MVP;
uniform mat4 MV;
uniform float time;


out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

float rand(float seed){
    return fract(sin(dot(vec2(seed) ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    fragNormal = normalize(transpose(inverse(mat3(MV))) * vertNormal);
    fragVert = vec3(MV * vec4(vert, 1.0f));
    fragTexCoord = vertTextCoord;


    gl_Position = MVP *vec4(vert, 1.0f);
}
