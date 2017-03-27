#version 130
in vec4 vPosition;
in vec4 vColor;
out vec4 color;
void main ()
{
    gl_Position =vec4(vPosition.x,vPosition.y,vPosition.z,0.7);
//    gl_Position =vPosition;
    color = vColor;
}
