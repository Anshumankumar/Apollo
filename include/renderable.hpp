#ifndef RENDERABLE
#define RENDERABLE

#include <vector>
#include <utils.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderable
{
    std::vector<Point> points;
    public:
    GLuint vao,vbo;
    void setPoints();
    void initVboVao();
};
#endif //RENDERABLE
