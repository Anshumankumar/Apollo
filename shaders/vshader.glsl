#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTex;

uniform mat4 transMat;
out vec4 color;
void main ()
{
    gl_Position =transMat*vPosition;
    color = vColor;
}

