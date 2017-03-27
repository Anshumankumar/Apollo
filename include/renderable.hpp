#ifndef RENDERABLE
#define RENDERABLE

#include <vector>
#include <utils.hpp>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderable
{
    protected:
    std::vector<Point> points;
    GLuint vao,vbo,program;
    public:
    void initialize(GLuint program);
    virtual void setPoints()=0;
    void render();
    virtual void modifyPoints();
};


class Triangle:public Renderable
{
    public:
    void setPoints();
    void modifyPoints();
};

class FileRender:public Renderable
{
    std::string filename;
    public:
    FileRender(std::string filename);
    void setPoints();
};

#endif //RENDERABLE
