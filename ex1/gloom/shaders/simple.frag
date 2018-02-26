#version 430 core

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform mat4 model;
uniform mat4 camera;
uniform vec3 cameraPosition;
uniform vec3 light;
uniform float ambientIntensity;


layout(binding=0) uniform sampler2D materialSampler;

out vec4 color;


void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(materialSampler, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);


    vec3 ambient = ambientIntensity * surfaceColor.rgb;

    float diffuseIntensity = max(0.0, dot(normal, light));
    vec3 diffuse = diffuseIntensity * surfaceColor.rgb;
    
    float specularIntensity = 0.0;

    if (diffuseIntensity > 0.0) specularIntensity = pow(max(0.0, dot(surfaceToCamera, reflect(light, normal))), 100);
    
    vec3 specular = vec3(specularIntensity);

    color = vec4( ambient +  diffuse + specular , 1.0f);
}
