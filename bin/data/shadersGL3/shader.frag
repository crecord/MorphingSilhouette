#version 150

uniform sampler2DRect tex0;
in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);
    outputColor = vec4(texel0.a,texel0.a,texel0.a, 1);
    //vec4(30,20,20,1);
}