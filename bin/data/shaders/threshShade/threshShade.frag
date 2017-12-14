#version 150

uniform sampler2DRect tex0;
uniform vec4 blobCol;
uniform vec4 backCol;
uniform vec4 highCol;
uniform float thresholdPnt;

uniform float motionBlrBt;
uniform float motionBlrTp;

uniform float alphaVal;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);
    
    if(texel0.r > thresholdPnt){
        texel0 = blobCol;
    }
    else{
        
        
        float scaled = motionBlrTp + texel0.r * (motionBlrBt - motionBlrTp);
        
        if(scaled < 0){
            scaled = 0; 
        }
        
        texel0 = vec4(scaled,scaled,scaled, texel0.a);
        
        texel0 = vec4(backCol.r + texel0.r * (highCol.r - backCol.r), backCol.g + texel0.g * (highCol.g - backCol.g), backCol.b + texel0.b * (highCol.b - backCol.b), alphaVal);
        
        //texel0 = vec4(backCol.r,backCol.g,backCol.b,backCol.a);
    }
    outputColor = texel0;
}