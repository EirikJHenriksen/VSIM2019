#version 330 core

in vec3 col;
in vec4 normal;
in vec4 fragPos;

uniform vec3 cameraPosition;

uniform vec3 lightPos = vec3(3.f, 1.f, -3.f);
uniform vec3 lightColor = vec3(1.f, 0.5f, 0.5f);

uniform float lightPower = 0.5f;
uniform float ambientStrength = 0.5f;
uniform float specularStrength = 0.9f;
uniform int specularExponent = 16;

out vec4 fragColor;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec4 norm = normalize(normal);
    vec4 lightDir = normalize(vec4(lightPos, 1.f) - fragPos);
    float diff = max(dot(norm, lightDir), 0.f);
    //vec4 diffuse = diff * vec4(lightColor, 1.f);// * lightPower;
    vec4 diffuse = diff * vec4(lightColor, 1.f) * lightPower;

    vec4 viewDir = normalize(vec4(cameraPosition, 1.f) - fragPos);
    vec4 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), specularExponent);
    vec4 specular = specularStrength * spec * vec4(lightColor, 1.f);

    vec4 result = 0.5f * (vec4(ambient * (fragPos.y * 0.1f), 1.f) + diffuse + specular) + 0.5f * vec4(col, 1.f);
    //vec4 result = 0.5f * (vec4(ambient, 1.f) + diffuse + specular) + 0.5f * vec4(col, 1.f);
    //vec4 result = (vec4(ambient, 1.f) + diffuse + specular) + vec4(col, 1.f);

    fragColor = result;
}
