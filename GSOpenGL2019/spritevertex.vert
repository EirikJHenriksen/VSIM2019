#version 330 core
layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec2 vertexUV;

out vec4 col;
out vec2 UV;

out vec4 FragPos;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

uniform vec3 objectColor = vec3(1.f, 1.f, 1.f);

uniform vec3 billboardPosition = vec3(1.f, 1.f, 1.f);
uniform vec3 billboardScale = vec3(1.f, 1.f, 1.f);

void main()
{
    UV = vertexUV;

    col = mat4(transpose(inverse(mMatrix))) * vec4(objectColor, 1.f);

    vec3 vertexPosition_worldspace = billboardPosition + vec3(vMatrix[0][0], vMatrix[1][0], vMatrix[2][0]) * posAttr.x * billboardScale.x + vec3(vMatrix[0][1], vMatrix[1][1], vMatrix[2][1]) * posAttr.y * billboardScale.y;
    FragPos = mMatrix * posAttr;

    gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition_worldspace, 1.f);
}
