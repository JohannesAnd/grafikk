#version 430 core

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform mat4 MVP;
uniform mat4 MV;

uniform vec3 cameraPosition;
uniform vec3 light;
uniform float ambientIntensity;


layout(binding=0) uniform sampler2D materialSampler;

out vec4 color;


void main()
{
    vec3 normal = fragNormal;
    vec3 surfacePos = fragVert;
    vec4 surfaceColor = texture(materialSampler, fragTexCoord);
    vec3 surfaceToCamera = normalize(vec3(MV*vec4(cameraPosition, 1.0f)) - surfacePos);
    vec3 surfaceToLight = -normalize(vec3(MV*vec4(light, 1.0f)) - surfacePos);


    float diffuseIntensity = max(0.0, dot(normal, surfaceToLight));
    
    float specularIntensity = 0.0;

    if(diffuseIntensity > 0.0) {
        specularIntensity = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), 1000);
    }
    
    color = vec4((ambientIntensity + diffuseIntensity + specularIntensity) * surfaceColor.rgb , 1.0f);
}
