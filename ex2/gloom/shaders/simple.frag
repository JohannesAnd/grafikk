#version 430 core

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;
in mat3 TBN;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;

uniform vec3 cameraPosition;
uniform vec3 light;
uniform float ambientIntensity;


layout(binding=0) uniform sampler2D textureSampler;
layout(binding=1) uniform sampler2D ambientSampler;
layout(binding=3) uniform sampler2D normalSampler;

out vec4 color;


void main()
{
    vec4 surfaceColor = texture(textureSampler, fragTexCoord);
    float ambientOcclusionValue = texture(ambientSampler, fragTexCoord).x;

    vec3 normalMap = texture(normalSampler, fragTexCoord).rgb;
    vec3 normalizedNormalMap = normalize(normalMap * 2.0 - 1.0);
    vec3 normal = normalize(TBN * normalizedNormalMap);

    vec3 surfacePos = fragVert;

    vec3 surfaceToCamera = normalize(vec3(MV*vec4(cameraPosition, 1.0f)) - surfacePos);
    vec3 surfaceToLight = normalize(vec3(V*vec4(light, 1.0f)) - surfacePos);

    float aperture = 2.0 * ambientOcclusionValue * ambientOcclusionValue;
    float ambientOcclusion = clamp(abs(dot(surfaceToLight, normal)) + aperture - 1, 0, 1);

    float diffuseIntensity = max(0.0, dot(normal, surfaceToLight));
    
    float specularIntensity = 0.0;

    if(diffuseIntensity > 0.0) {
        specularIntensity = pow(max(0.0, dot(surfaceToCamera, reflect(surfaceToLight, normal))), 1000);
    }

    color = vec4(vec3(ambientIntensity + diffuseIntensity + specularIntensity) * ambientOcclusion * surfaceColor.rgb , 1.0f);
}
