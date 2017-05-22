#ifndef UTILS_HPP
#define UTILS_HPP
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace apollo
{
struct TPoint
{
    float x,y,z,w;
    float cx,cy,cz,a;
    float nx,ny,nz;
    float tx,ty;
};

struct Point
{
    glm::vec4 x;
    glm::vec4 c;
    glm::vec3 n; 
    glm::vec2 t;
};
}
#endif 
