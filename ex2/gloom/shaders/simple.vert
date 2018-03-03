#version 430 core

layout(location=0) in vec3 vert;
layout(location=1) in vec2 vertTextCoord;
layout(location=2) in vec3 vertNormal;

uniform mat4 MVP;
uniform mat4 MV;


out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
    fragNormal = normalize(transpose(inverse(mat3(MV))) * vertNormal);
    fragVert = vec3(MV * vec4(vert, 1.0f));
    fragTexCoord = vertTextCoord;

    gl_Position = MVP * vec4(vert, 1.0f);
}
