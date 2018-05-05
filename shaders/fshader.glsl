#version 130

in vec4 color;
in  vec3 normal;
in vec4 position;

uniform vec3 light = vec3(1.0,1.0,-1.0);
uniform vec3 eye = vec3(0.0,0.0,-1.0);

out vec4 frag_color;
void main ()
{
    float a = max(0,dot(normal,normalize(vec3(light-vec3(position)))));
    float specular = 0;
    if (a>0)
    {
        vec3 reflect = reflect(normalize(vec3(position)-light),normalize(normal));
        vec3 eyeLocal = normalize(eye-vec3(position));
        specular = pow(max(0,dot(reflect,eyeLocal)),30.0);
    }
    frag_color = 0.4*a*color+ 0.2*color + 1.0 *specular*vec4(1.0,1.0,1.0,1.0);
    frag_color.w = 1.0;
}
