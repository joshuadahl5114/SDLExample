#version 150

in vec4 color;
out vec4 outColor;

void main(void) {
    if (color == vec4(1,1,1,0) || color == vec4(1,1,1,1) )
    {
        outColor = vec4(0,1,0,1);
    }
    else
    {
        outColor = color;
    }
    
}
