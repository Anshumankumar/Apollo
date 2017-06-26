#ifndef RENDERABLE
#define RENDERABLE

#define GLM_FORCE_RADIANS

#include <vector>
#include <utils.hpp>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <generator.hpp>
namespace apollo
{
class Renderable
{
    protected:
    std::vector<Point> points;
    GLuint vao,vbo,program;
    float scale =0;
    public:
    glm::mat4 tmat;
    void initialize(GLuint program);
    virtual void setPoints()=0;
    void render();
    virtual void modifyPoints();
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
}
#endif //RENDERABLE
