#version 330 core
layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 vertexUV;

out vec4 col;
out vec2 UV;

uniform vec3 objectColor = vec3(1.f, 1.f, 1.f);

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
        col = vec4(objectColor, 1.f);
        UV = vertexUV; //vec2(aTexCoord.x, aTexCoord.y);
        gl_Position = pMatrix * vMatrix * mMatrix * posAttr;
}
