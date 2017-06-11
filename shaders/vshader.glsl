#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTex;

uniform mat4 transMat;
out vec4 color;
out vec3 normal;
out vec4 position;

void main ()
{
    gl_Position =transMat*vPosition;
    position=transMat*vPosition;
    color = vColor;
    normal = (transMat*vec4(vNormal.x,vNormal.y,vNormal.z,0)).xyz;    
}
