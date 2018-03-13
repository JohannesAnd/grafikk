#version 430 core

layout(location=0) in vec3 vert;
layout(location=1) in vec2 vertTextCoord;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 biTangent;

layout(binding=2) uniform sampler2D animationSampler;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float time;


out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 TBN;

float rand(float seed){
    return fract(sin(dot(vec2(seed) ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    fragNormal = normalize(transpose(inverse(mat3(MV))) * vertNormal);
    fragVert = vec3(MV * vec4(vert, 1.0f));
    fragTexCoord = vertTextCoord;

    vec3 T = normalize(vec3(M * vec4(tangent,    0.0)));
    vec3 B = normalize(vec3(M * vec4(biTangent,  0.0)));
    vec3 N = normalize(vec3(M * vec4(vertNormal, 0.0)));
    
    TBN = mat3(T, B, N);

    gl_Position = MVP *vec4(vert, 1.0f);
}
