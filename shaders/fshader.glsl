#version 310 es
precision mediump float;
in vec4 color;
in  vec3 normal;
in vec4 position;
in vec2 uv;

uniform sampler2D myTextureSampler;
vec3 light = vec3(1.0,1.0,-1.0);
vec3 eye = vec3(0.0,0.0,-1.0);

out vec4 frag_color;
void main ()
{
    vec3 normal2 =   0.5*vec3(texture(myTextureSampler, uv).rrr);
    vec3 normal3 =  normal + 0.1*vec3(texture(myTextureSampler, uv).bgr);
    vec4 color2 =  vec4(texture(myTextureSampler, uv).rgb,1.0);
    float a = max(0.0,dot(normal2,normalize(vec3(light-vec3(position)))));
    float specular = 1.0;
    if ((a>0.0))
    {
        vec3 reflect = reflect(normalize(vec3(position)-light),normalize(normal3));
        vec3 eyeLocal = normalize(eye-vec3(position));
        specular = pow(max(0.0,dot(reflect,eyeLocal)),30.0);
    }
    else
    {
        specular = 0.0;
    }
    //frag_color = 2.0*a*color2; //+ 0.8*color2 ;//+ 0.4 *specular*vec4(1.0,1.0,1.0,1.0);
    frag_color = color2 ;
    frag_color.w = 1.0;
}
