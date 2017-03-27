#version 130
in vec4 vPosition;
in vec4 vColor;
uniform float gScale=1;
out vec4 color;
void main ()
{
    gl_Position =vec4(gScale*vPosition.x, gScale*vPosition.y,vPosition.z,vPosition.w);
//    gl_Position =vPosition;
    color = vColor;
}
