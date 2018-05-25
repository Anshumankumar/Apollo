#version 310 es
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTex;
in vec3 rVec;

uniform mat4 transMat;
out vec4 color;
out vec3 normal;
out vec4 position;
out vec2 uv;

void main () {
    gl_Position =transMat*(vPosition);
    position=transMat*(vPosition);
    color = vColor;
    normal = (transMat*vec4(vNormal.x,vNormal.y,vNormal.z,0)).xyz;    
    uv = vTex;
}
