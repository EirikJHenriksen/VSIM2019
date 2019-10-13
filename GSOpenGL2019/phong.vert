#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 inNormal; // Normals of mesh

out vec3 col;
out vec4 normal;
out vec4 fragPos;

uniform vec3 objectColor = vec3(1.f, 0.5f, 0.5f);

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

void main() {
    col = objectColor;
    fragPos = mMatrix * posAttr;

    normal = mat4(transpose(inverse(mMatrix))) * inNormal;

    gl_Position = pMatrix * vMatrix * fragPos;
}
