#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
//layout(location = 2) in vec2 UV;

out vec4 col;
uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

void main() {
    col = colAttr;
    //col = UV;
    gl_Position = pMatrix * vMatrix * mMatrix * posAttr;
}
