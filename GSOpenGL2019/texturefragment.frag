#version 330 core
out vec4 colAttr;

in vec4 col;
in vec2 UV;

uniform sampler2D textureSampler;

void main()
{
    //colAttr = texture(textureSampler, UV) * col;
    colAttr = texture(textureSampler, UV) * col;
}
