#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <utils.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

namespace apollo
{
     Point getPointEllipse(float a,float b,float c,float theta, float phi);

class Generator
{
    protected:
    std::vector <Point> points;
    
    public:
    std::vector<Point> getPoints();
    void savePoints(std::string filename);
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
    void addSquarePoint(int index, std::vector <Point> &vertices);
};

}
#endif // GENERATOR_HPP
