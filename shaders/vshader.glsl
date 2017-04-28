#version 130

in vec4 vPosition;
in vec4 vColor;
uniform float gScale=1;
uniform mat4 transMat;
out vec4 color;
void main ()
{
//    gl_Position =vec4(gScale*vPosition.x, gScale*vPosition.y,vPosition.z,vPosition.w);
    gl_Position =transMat*vPosition;
    color = vColor;
}

