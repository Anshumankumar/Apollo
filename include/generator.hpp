#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <utils.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <params/params.hpp>

namespace apollo
{
Point getPointEllipse(float a,float b,float c,float theta, float phi,
        glm::vec4 color,float zShift=0);

class Generator
{
    protected:
    std::vector <Point> points;
    Params params; 
    glm::vec4 color;
    public:
    std::vector<Point> getPoints();
    void savePoints(std::string filename);
    void scale(float x=1, float y=1, float z=1);
    void rotate(float x=1, float y=1, float z=1);
    void translate(float x=0, float y=0, float z=0);
    void shearX(float mag);
    void modify();
    void getColor();
};

class PartEllipsoid:public Generator
{    
    public:

    PartEllipsoid(float a,float b, float c, float factor);
}; 

class Circle:public Generator
{
    public:
    Circle(float radius);
};

class Cube:public Generator
{
    public:
    Cube(float side);
    void addSquarePoint(int a, int b, int c, int d, std::vector <Point> vertices);
    void addSquarePoint(int index, std::vector <Point> vertices);
};

class Icosahedron:public Generator{
    public:
    Icosahedron(float len);
};

class Sphere:public Generator
{
    public:
    Sphere(float radius);
    
};

class Frustum:public Generator
{
    public:
    Frustum(float rTop, float rBottom, float height);
    
};

class Triangle:public Generator
{
    public:
    Triangle();
};

class Combiner:public Generator 
{
    public:
    Combiner(std::vector<Generator*>);
};
}
#endif // GENERATOR_HPP
