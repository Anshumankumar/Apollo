#ifndef RENDERABLE
#define RENDERABLE

#include <vector>
#include <utils.hpp>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <generator.hpp>

class Renderable
{
    protected:
    std::vector<Point> points;
    GLuint vao,vbo,program;
    float scale =0;
    public:
    float tmat[4][4] = {0};
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


class GeneratorRender:public Renderable
{
    Generator *generator;
    public:
    GeneratorRender(Generator * generator);
    void modifyPoints();
    void setPoints();
};


class FileRender:public Renderable
{
    std::string filename;
    public:
    FileRender(std::string filename);
    void setPoints();
};

#endif //RENDERABLE
