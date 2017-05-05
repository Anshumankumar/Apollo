#ifndef UTILS_HPP
#define UTILS_HPP
#include <glm/vec4.hpp>
namespace apollo
{
struct Point
{
    float x,y,z,w;
    float cx,cy,cz,a;;
};

struct GPoint
{
    glm::vec4 x;
    glm::vec4 c; 
};
}
#endif 
