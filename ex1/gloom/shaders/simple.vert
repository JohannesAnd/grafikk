#version 430 core

layout(location=0) in vec3 vert;
layout(location=1) in vec2 vertTextCoord;
layout(location=2) in vec3 vertNormal;

uniform mat4 model;
uniform mat4 camera;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
    fragVert = vert;
    fragTexCoord = vertTextCoord;
    fragNormal = vertNormal;

    gl_Position = model * camera * vec4(vert, 1.0f);
}
